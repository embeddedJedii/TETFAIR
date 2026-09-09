#include <Arduino.h>
#include "timerCustom.h"
#include "temperature.h"
uint8_t clockwiseLimitSwitchPin = 12;
uint8_t counterClockwiseLimitSwitchPin = 15;
volatile uint8_t MOTOR_PIN_CLOCKWISE = 4;
volatile uint8_t MOTOR_PIN_COUNTERCLOCKWISE = 13;
volatile bool limitSwitchTriggered = false;
volatile bool limitSwitchTriggered1 = false;
bool motorTurning = false;
bool turnClockwise = true;
uint32_t lastTurnTime;
uint8_t turningClockwise = 0;
uint8_t turningCounterClockwise = 0;
// const uint32_t TURN_INTERVAL = 4UL * 60UL * 60UL;
const uint32_t TURN_INTERVAL = 20;  // 10 seconds for testing purposes
void motorPinsInit() {
    pinMode(MOTOR_PIN_CLOCKWISE, OUTPUT);
    pinMode(MOTOR_PIN_COUNTERCLOCKWISE, OUTPUT);
    digitalWrite(MOTOR_PIN_CLOCKWISE, LOW);
    digitalWrite(MOTOR_PIN_COUNTERCLOCKWISE, LOW);
}
void IRAM_ATTR limitSwitchISR()
{
    limitSwitchTriggered = true;
}

void IRAM_ATTR limitSwitchISR1()
{
    limitSwitchTriggered1 = true;
}
void limitSwitchInit()
{
    pinMode(counterClockwiseLimitSwitchPin, INPUT);
    pinMode(clockwiseLimitSwitchPin, INPUT);
    attachInterrupt(
        digitalPinToInterrupt(counterClockwiseLimitSwitchPin),
        limitSwitchISR,
        RISING
    );

    attachInterrupt(
        digitalPinToInterrupt(clockwiseLimitSwitchPin),
        limitSwitchISR1,
        RISING
    );
}

// void turningLogic()
// {
//     // if(!incubationActive) {
//     //     return;
//     // } 
//     // CLOCKWISE movement
//     if (turnClockwise)
//     {
//         Serial.println("Turning clockwise...");
//         if (digitalRead(clockwiseLimitSwitchPin) == HIGH)
//         {
//             // Stop motor
//             digitalWrite(MOTOR_PIN_CLOCKWISE, LOW);
//             digitalWrite(MOTOR_PIN_COUNTERCLOCKWISE, LOW);

//             motorTurning = false;
//             turnClockwise = false; 

//             // Start the 4-hour waiting period
//             lastTurnTime = rtc.now().unixtime();

//             Serial.println("Clockwise limit reached.");
//             Serial.println("Motor stopped.");
//         }
//         else
//         {
//             // Make sure anticlockwise relay is OFF
//             digitalWrite(MOTOR_PIN_COUNTERCLOCKWISE, LOW);

//             // Run clockwise
//             digitalWrite(MOTOR_PIN_CLOCKWISE, HIGH);
//             Serial.println("Turning eggs clockwise...");
//         }
//     } 

//     // ANTICLOCKWISE movement
//     else
//     {
//         if (digitalRead(counterClockwiseLimitSwitchPin) == HIGH)
//         {
//             // Stop motor
//             digitalWrite(MOTOR_PIN_COUNTERCLOCKWISE, LOW);
//             digitalWrite(MOTOR_PIN_CLOCKWISE, LOW);

//             motorTurning = false;
//             turnClockwise = true;

//             // Start the 4-hour waiting period
//             lastTurnTime = rtc.now().unixtime();
 
//             Serial.println("Anticlockwise limit reached.");
//             Serial.println("Motor stopped.");
//         }
//         else
//         {
//             // Make sure clockwise relay is OFF
//             digitalWrite(MOTOR_PIN_CLOCKWISE, LOW);

//             // Run anticlockwise
//             digitalWrite(MOTOR_PIN_COUNTERCLOCKWISE, HIGH);
//               Serial.println("Turning eggs anticlockwise...");
//         }
//     }
// }

void turningLogic()
{
    // =========================
    // CLOCKWISE MOVEMENT
    // =========================

    if (turnClockwise)
    {
        // Check if clockwise limit was triggered
        if (limitSwitchTriggered1)
        {
            // Stop both relays
            digitalWrite(MOTOR_PIN_CLOCKWISE, LOW);
            digitalWrite(MOTOR_PIN_COUNTERCLOCKWISE, LOW);

            // Clear interrupt flag
            limitSwitchTriggered1 = false;

            // Automatic turning finished
            motorTurning = false;

            // Next automatic direction = anticlockwise
            turnClockwise = false;

            // Start new 4-hour waiting period
            lastTurnTime = rtc.now().unixtime();

            Serial.println("Clockwise limit reached.");
            Serial.println("Motor stopped.");
        }
        else
        {
            // Ensure opposite relay is OFF
            digitalWrite(MOTOR_PIN_COUNTERCLOCKWISE, LOW);

            // Run clockwise
            digitalWrite(MOTOR_PIN_CLOCKWISE, HIGH);

            Serial.println("Turning eggs clockwise...");
        }
    }


    // =========================
    // ANTICLOCKWISE MOVEMENT
    // =========================

    else
    {
        // Check if anticlockwise limit was triggered
        if (limitSwitchTriggered)
        {
            // Stop both relays
            digitalWrite(MOTOR_PIN_COUNTERCLOCKWISE, LOW);
            digitalWrite(MOTOR_PIN_CLOCKWISE, LOW);

            // Clear interrupt flag
            limitSwitchTriggered = false;

            // Automatic turning finished
            motorTurning = false;

            // Next automatic direction = clockwise
            turnClockwise = true;

            // Start new 4-hour waiting period
            lastTurnTime = rtc.now().unixtime();

            Serial.println("Anticlockwise limit reached.");
            Serial.println("Motor stopped.");
        }
        else
        {
            // Ensure opposite relay is OFF
            digitalWrite(MOTOR_PIN_CLOCKWISE, LOW);

            // Run anticlockwise
            digitalWrite(MOTOR_PIN_COUNTERCLOCKWISE, HIGH);

            Serial.println("Turning eggs anticlockwise...");
        }
    }
}

void eggTurningControl()
{

    // MANUAL CONTROL

    if (!motorTurning)
    {
        if (turningClockwise == 1)
        {
            digitalWrite(MOTOR_PIN_COUNTERCLOCKWISE, LOW);

            if (digitalRead(clockwiseLimitSwitchPin) == LOW)
            {
                digitalWrite(MOTOR_PIN_CLOCKWISE, HIGH);
            }
            else
            {
                digitalWrite(MOTOR_PIN_CLOCKWISE, LOW);
            }
        }

        else if (turningCounterClockwise == 1)
        {
            digitalWrite(MOTOR_PIN_CLOCKWISE, LOW);

            if (digitalRead(counterClockwiseLimitSwitchPin) == LOW)
            {
                digitalWrite(MOTOR_PIN_COUNTERCLOCKWISE, HIGH);
            }
            else
            {
                digitalWrite(MOTOR_PIN_COUNTERCLOCKWISE, LOW);
            }
        }
    }



    // AUTOMATIC CONTROL
    // tURN OFF MOTOR DURING HATCHING


    if (!incubationActive)
        return;

    // Stop AUTOMATIC turning during hatching period.
    if (incubationDay >= hatchingDay)
        return;

    uint32_t currentTime = rtc.now().unixtime();

    // Start a new automatic turn after 4 hours
    if (!motorTurning &&
        currentTime - lastTurnTime >= TURN_INTERVAL)
    {
        Serial.println("Turning eggs...");

        motorTurning = true;
    }

    // Continue automatic movement
    if (motorTurning)
    {
        turningLogic();
    }
}