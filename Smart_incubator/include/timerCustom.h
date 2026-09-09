#pragma once
#include <RTClib.h>
#include <Preferences.h>
#include <timerCustom.h>
extern uint32_t elapsedSeconds;
extern int incubationDay;
extern uint32_t incubationStartTime;
extern bool incubationActive;
extern RTC_DS3231 rtc;
extern Preferences preferences;
extern int TOTAL_INCUBATION_DAYS;
extern uint8_t circulationFan;
void ciculationFanInit ();
void syncRTCviaNTP();
void RTCInit();
void startIncubation();
void stopIncubation();
void loadIncubation();
void updateIncubation();
void printIncubationStatus();
void printUpdateIncubationStatus();
uint16_t sendIncubationDays();