#include <Arduino.h>
#include "motor.h"
#define clockwiseLimitSwitchPin 26
#define counterClockwiseLimitSwitchPin 33

void setup() {
  // put your setup code here, to run once:

  pinMode(clockwiseLimitSwitchPin, INPUT_PULLUP);
  pinMode(counterClockwiseLimitSwitchPin, INPUT_PULLUP);
  driveMotor();
  //attachInterrupt(digitalPinToInterrupt(clockwiseLimitSwitchPin),driveMotor(), FALLING);

}

void loop() {
  // put your main code here, to run repeatedly:
}

