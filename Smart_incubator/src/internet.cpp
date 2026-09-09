#include <Arduino.h>
#include <WiFi.h>
#include "i2c.h"
const char* ssids = receivedCommand.SSID; //Received from CYD
const char* passwords = receivedCommand.Password;

void connectToWifi (){
    WiFi.begin(ssids, passwords);
  Serial.print("Connecting to WiFi");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
  Serial.println(" connected");
}
void isConnected(){
     if(WiFi.status() == WL_CONNECTED){
        Serial.println("Wifi is connected....");
     }else{
          WiFi.begin(ssids, passwords);
          Serial.println("Still trying to connect");
     }
}
