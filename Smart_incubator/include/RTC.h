#ifndef RTC_H
#define RTC_H

#include <Arduino.h>
#include <RTClib.h>

extern RTC_DS3231 rtc;
extern char daysOfTheWeek[7][12];

extern void RTCInit();
extern void getDate();

#endif // RTC_H
