#ifndef TURNING_SYSTEM_H
#define TURNING_SYSTEM_H

#include <Arduino.h>

extern uint8_t clockwiseLimitSwitchPin;
extern uint8_t counterClockwiseLimitSwitchPin;
extern volatile uint8_t motorPinClockwise;
extern volatile uint8_t motorPinCounterClockwise;

void  driveMotorAuto();

void IRAM_ATTR shutdownMotorClockwise();
void IRAM_ATTR shutdownMotorCounterClockwise();
#endif // TURNING_SYSTEM_H
