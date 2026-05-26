#include <Arduino.h>
#include "turningSystem.h"

uint8_t clockwiseLimitSwitchPin = 34;
uint8_t counterClockwiseLimitSwitchPin = 33;
volatile uint8_t motorPinClockwise = 21;
volatile uint8_t motorPinCounterClockwise = 26;

void  driveMotorAuto() {
// Check if the anticlockwise limit switch is active, if yes, move the motor clockwise.
uint8_t lastState;
if(!digitalRead(counterClockwiseLimitSwitchPin)) {
  digitalWrite(motorPinClockwise, HIGH);
  lastState = 0;
}
// Check if the clockwise limit switch is active, if yes, move anticlockwise
else if(!digitalRead(clockwiseLimitSwitchPin)) {
  digitalWrite(motorPinCounterClockwise, HIGH);
  lastState = 1;
}
// check if neither is active, if neither is active, the movementr will be based on the last state
else {
  lastState == 0? digitalWrite(motorPinCounterClockwise, HIGH) : digitalWrite(motorPinClockwise, HIGH);
}
}

void IRAM_ATTR shutdownMotorClockwise() {
  digitalWrite(motorPinClockwise, LOW);
}

void IRAM_ATTR shutdownMotorCounterClockwise() {
  digitalWrite(motorPinCounterClockwise, LOW);
}

