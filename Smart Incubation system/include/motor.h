#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

extern volatile uint8_t MOTOR_PIN;
void IRAM_ATTR driveMotor();

#endif