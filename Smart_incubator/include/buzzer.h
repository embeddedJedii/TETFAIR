#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

extern uint8_t buzzerPin;
void buzzerInit();
void buzzerOn();
void buzzerOff();

#endif // BUZZER_H
