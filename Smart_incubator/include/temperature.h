#pragma once

#include <Wire.h>
#include <Adafruit_SHT31.h>

constexpr int SDA1_PIN = 21;
constexpr int SCL1_PIN = 22;
constexpr int SDA2_PIN = 18;
constexpr int SCL2_PIN = 19;

extern TwoWire i2cBUS2;
extern Adafruit_SHT31 sht1;
extern Adafruit_SHT31 sht2;
extern uint8_t hatchingDay;
extern uint8_t hatchingHumidity;
extern uint8_t HUMIDIFIER_PIN;
extern uint8_t HEATER_PIN;
extern uint8_t EXHAUST_FAN_PIN;

void tempPinInit();
void tempHumidInit();
float getTemp();
float getHumidity();
void heaterLogic();
void humidifierInit();
void humidifierLogic();
void hatchingAlgorithm();
void humidifierLogicHatching();
void checkTempHumFault();
void checkTempHumOvershoot();