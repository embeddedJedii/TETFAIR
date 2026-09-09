#ifndef INTERNET_H
#define INTERNET_H

#include <Arduino.h>

extern const char* ssids;
extern const char* passwords;

void connectToWifi();
void isConnected();

#endif // INTERNET_H
