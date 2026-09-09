#ifndef TURNING_SYSTEM_H
#define TURNING_SYSTEM_H

#include <Arduino.h>

extern uint8_t clockwiseLimitSwitchPin;
extern uint8_t counterClockwiseLimitSwitchPin;
extern volatile uint8_t MOTOR_PIN_CLOCKWISE;
extern volatile uint8_t MOTOR_PIN_COUNTERCLOCKWISE;
extern volatile bool limitSwitchTriggered;
extern volatile bool limitSwitchTriggered1;
extern bool motorTurning;
extern bool turnClockwise;
extern uint32_t lastTurnTime;
void IRAM_ATTR limitSwitchISR();
void IRAM_ATTR limitSwitchISR1();
void limitSwitchInit();
void motorPinsInit();
// void motorAction();
void turningLogic ();
void eggTurningControl();
#endif // TURNING_SYSTEM_H
