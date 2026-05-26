#include <Arduino.h>
#include <wire.h>
#include <Adafruit_SHT31.h>
#include "turningSystem.h"
#include "sht31.h"
#include "RTC.h"
#define SDA_PIN 8
#define SCL_PIN 9
void setup() {
  // put your setup code here, to run once:

  pinMode(clockwiseLimitSwitchPin, INPUT_PULLUP);
  pinMode(counterClockwiseLimitSwitchPin, INPUT_PULLUP);
  pinMode(motorPinClockwise, OUTPUT);
  pinMode(motorPinCounterClockwise, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(clockwiseLimitSwitchPin), shutdownMotorClockwise, FALLING);
  attachInterrupt(digitalPinToInterrupt(counterClockwiseLimitSwitchPin), shutdownMotorCounterClockwise, FALLING);
  Serial.begin(115200);
  while (!Serial) delay(10);
  // Initialize I2C with ESP32-S3 pins
  // Wire.begin(SDA_PIN, SCL_PIN);
  shtStatus();
  RTCInit();
  Serial.println("SHT31 initialized successfully.");
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println("I am here");
getDate();
getTemperature();
getHumidity();
  delay(2000);
}

 