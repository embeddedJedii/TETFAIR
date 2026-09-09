#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include "temperature.h"
#include "I2Cinit.h"
#include "buzzer.h"
#include "timerCustom.h"
// TwoWire i2cBUS2 = TwoWire(1);   // second hardware I2C peripheral
uint8_t hatchingDay = 18; // Replace this with the hatching data coming from the CYD.
uint8_t hatchingHumidity = 0;
Adafruit_SHT31 sht1 = Adafruit_SHT31(&Wire);      // sensor 1 on bus 1
Adafruit_SHT31 sht2 = Adafruit_SHT31(&I2Cbus2);   // sensor 2 on bus 2
uint8_t HUMIDIFIER_PIN = 14; // Pin to control the humidifier
uint8_t HEATER_PIN = 5; // Pin to control the heater
uint8_t EXHAUST_FAN_PIN = 26; // Pin to control the exhaust fan
float setTemp = 32.0;
float setHumid = 60.0;
float temp;
float humid;
void tempPinInit(){
  pinMode(HEATER_PIN, OUTPUT);
  digitalWrite(HEATER_PIN, LOW);
  pinMode(HUMIDIFIER_PIN, OUTPUT);
  digitalWrite(HUMIDIFIER_PIN, LOW);
  pinMode(EXHAUST_FAN_PIN, OUTPUT);
  digitalWrite(EXHAUST_FAN_PIN, LOW);
}
void tempHumidInit(){
    Wire.begin(SDA1_PIN, SCL1_PIN);
  if (!sht1.begin(0x44)) {
    Serial.println("SHT31 #1 not found on bus 1 (21/22)");
  } else {
    Serial.println("SHT31 #1 OK on bus 1 (21/22)");
  }

  // Init bus 2 (SHT31 #2 + RTC)
  I2Cbus2.begin(SDA2_PIN, SCL2_PIN);
  if (!sht2.begin(0x44)) {
    Serial.println("SHT31 #2 not found on bus 2 (18/19)");
  } else {
    Serial.println("SHT31 #2 OK on bus 2 (18/19)");
  }

}

float getTemp(){
  float t1 = sht1.readTemperature();
  float t2 = sht2.readTemperature();
 
      if (!isnan(t1))
    Serial.printf("SHT1: %.2fC", t1);
  else
    Serial.print("SHT1: read Temp fail  ");  

  if (!isnan(t2))
    Serial.printf("SHT2: %.2fC", t2);
  else
    Serial.print("SHT2: read temp fail");

  Serial.println();
    return (t1 + t2) / 2;  // Return the average temperature from both sensors
}

float getHumidity(){
  float h1 = sht1.readHumidity();
  float h2 = sht2.readHumidity();
        if (!isnan(h1))
    Serial.printf("SHT1: %.2f%%", h1);
  else
    Serial.print("SHT1: read Humidity fail  ");

  if (!isnan(h2))
    Serial.printf("SHT2: %.2f%%", h2);
  else
    Serial.print("SHT2: read Humidity fail");

  Serial.println();
  return (h1 + h2) / 2;  // Return the average humidity from both sensors 
}
void checkTempHumFault(){
  float t1 = sht1.readTemperature();
  float t2 = sht2.readTemperature();
  float h1 = sht1.readHumidity();
  float h2 = sht2.readHumidity();

  if (isnan(t1) || isnan(t2) || isnan(h1) || isnan(h2)) {
    Serial.println("Temperature or Humidity sensor fault detected!");
    buzzerOn(); // Activate buzzer on fault
    delay(1000); 
    buzzerOff();
    delay(1000);
  } else {
    buzzerOff(); // Deactivate buzzer if no fault
  }
}

void checkTempHumOvershoot(){
  float temp = getTemp();
  float humid = getHumidity();

  if(temp > setTemp + 5.0 || humid > setHumid + 10.0) { //This can be changed according to the specified humidity.
    Serial.println("Temperature or Humidity overshoot detected!");
    buzzerOn(); // Activate buzzer on overshoot
    delay(1000); 
    buzzerOff();
    delay(1000);
  } else {
    buzzerOff(); // Deactivate buzzer if no overshoot
  }
}


void heaterLogic()
{
    const float HYSTERESIS = 0.5;

    float temp = getTemp();

    if (temp <= setTemp - HYSTERESIS)
    {
        // Temperature is significantly below setpoint
        Serial.println("Temperature low. Turning heater ON.");

        digitalWrite(HEATER_PIN, HIGH);
        digitalWrite(EXHAUST_FAN_PIN, LOW);
    }
    else if (temp >= setTemp + HYSTERESIS)
    {
        // Temperature is significantly above setpoint
        Serial.println("Temperature high. Turning heater OFF.");

        digitalWrite(HEATER_PIN, LOW);
        digitalWrite(EXHAUST_FAN_PIN, HIGH);
    }
}

void humidifierInit(){
    // Initialize the humidifier pin as OUTPUT
    pinMode(HUMIDIFIER_PIN, OUTPUT);
    digitalWrite(HUMIDIFIER_PIN, LOW); // Ensure the humidifier is off initially
}
void humidifierLogic(){
     uint16_t humidity = getHumidity();
     if(humidity < setHumid) {
        // Turn on humidifier
        Serial.println("Humidity below setpoint. Turning on humidifier.");
        digitalWrite(HUMIDIFIER_PIN, HIGH);
     }else{
        // Turn off humidifier 


        Serial.println("Humidity above setpoint. Turning off humidifier.");
        digitalWrite(HUMIDIFIER_PIN, LOW);
     }
}
void humidifierLogicHatching(){
     uint16_t humidity = getHumidity();
     if(humidity < hatchingHumidity) {
        // Turn on humidifier
        Serial.println("Humidity below setpoint. Turning on humidifier."); 
        digitalWrite(HUMIDIFIER_PIN, HIGH);
     }else{
        // Turn off humidifier
        Serial.println("Humidity above setpoint. Turning off humidifier.");
        digitalWrite(HUMIDIFIER_PIN, LOW);
     }
}
void hatchingAlgorithm(){
    uint16_t daysIntoIncubation = sendIncubationDays();
    uint16_t humidity = getHumidity();
    // Increase the humidity and turn the motor completely off;
    if (daysIntoIncubation >= hatchingDay) {
            // Increase humidity logic here i.e turn on the humidifier and turn off the motor
            Serial.println("Hatching phase: Increase humidity.");
            humidifierLogicHatching(); // Call the humidifier logic to increase humidity
        Serial.println("Hatching phase: Increase humidity and turn off motor.");
    }else {
        // Normal incubation logic here
       
        Serial.println("Normal incubation phase.");
        humidifierLogic(); // Call the normal humidifier logic
    }
}

