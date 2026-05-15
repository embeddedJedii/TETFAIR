#include <Arduino.h>


volatile uint8_t MOTOR_PIN = 21;

void IRAM_ATTR driveMotor() {
  // Code to drive the motor clockwise
  digitalWrite(MOTOR_PIN, HIGH);
}
