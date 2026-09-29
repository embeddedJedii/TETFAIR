#ifndef INTERNET_H
#define INTERNET_H

#include <Arduino.h>
#include <FirebaseClient.h>
extern const char* ssids;
extern const char* passwords;
extern uint8_t wifiCode;

void connectToWifi();
void isConnected();
void initializeFirebase();
void sendDataToDB();
void processData(AsyncResult &aResult);
#endif // INTERNET_H



// #ifndef INTERNET_H
// #define INTERNET_H

// #define ENABLE_USER_AUTH
// #define ENABLE_DATABASE

// #include <Arduino.h>
// #include <FirebaseClient.h>

// // Wi-Fi status
// extern uint8_t wifiCode;

// // Wi-Fi functions
// void connectToWifi();
// void isConnected();

// // Firebase functions
// void initializeFirebase();
// void sendDataToDB();

// // Firebase callback
// void processData(AsyncResult &aResult);

// #endif // INTERNET_H