#include <Arduino.h>
#include "turningSystem.h"

void setup() {
  // put your setup code here, to run once:

  pinMode(clockwiseLimitSwitchPin, INPUT_PULLUP);
  pinMode(counterClockwiseLimitSwitchPin, INPUT_PULLUP);
  pinMode(motorPinClockwise, OUTPUT);
  pinMode(motorPinCounterClockwise, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(clockwiseLimitSwitchPin), shutdownMotorClockwise, FALLING);
  attachInterrupt(digitalPinToInterrupt(counterClockwiseLimitSwitchPin), shutdownMotorCounterClockwise, FALLING);

}

void loop() {
  // put your main code here, to run repeatedly:
}

