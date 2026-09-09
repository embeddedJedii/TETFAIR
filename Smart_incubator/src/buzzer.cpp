#include <Arduino.h>

uint8_t buzzerPin = 32;

void buzzerInit() {
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); // Ensure buzzer is off initially
}

void buzzerOn() {
  digitalWrite(buzzerPin, HIGH);
}

void buzzerOff() {
  digitalWrite(buzzerPin, LOW);
}