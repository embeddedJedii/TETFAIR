#define ENABLE_USER_AUTH
#define ENABLE_DATABASE

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include "i2c.h"
#include "temperature.h"
#include "secrets.h"
const char* ssids; //Received from CYD
const char* passwords;
// #define Web_API_KEY "AIzaSyDpORcQZmg_knlbM3H2Ep1g7YQ57SobY00"
// #define DATABASE_URL "https://smarteggincubator-5ee0f-default-rtdb.firebaseio.com"
// #define USER_EMAIL "ridwan.innov8@gmail.com"
// #define USER_PASS "Olasunkanmi1*"

void processData(AsyncResult &aResult);

// Authentication
UserAuth user_auth(Web_API_KEY, USER_EMAIL, USER_PASS);

// Firebase components
FirebaseApp app;
WiFiClientSecure ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);
RealtimeDatabase Database;

// Timer variables for sending data every 10 seconds
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 10000; // 10 seconds in milliseconds
float temperature = 32.5;
float humidity = 60.6;
float setHumidity = receivedCommand.setHumidity;
float setTemperature = receivedCommand.setTemp;
int totalIncubationDays = receivedCommand.incubationDays;
int wifiStatus = 1;
int hatchingDays = receivedCommand.hatchingDays;
int currentIncubationDay = 5;
uint8_t wifiCode = 0;
void connectToWifi (){
  WiFi.begin(ssids, passwords);
  Serial.print("Connecting to WiFi");
  Serial.println(" connected");
}
void isConnected(){
     if(WiFi.status() == WL_CONNECTED){
        Serial.println("Wifi is connected....");
        wifiCode = 1;
        return;
        
     }else{
          wifiCode = 0;
              ssids = receivedCommand.SSID;
              passwords = receivedCommand.Password;
              connectToWifi();
          Serial.println("Wifi is not connected");
     }
}

void initializeFirebase(){
    // Configure SSL client
  ssl_client.setInsecure();
  // ssl_client.setConnectionTimeout(1000);
  ssl_client.setHandshakeTimeout(5);
  
  // Initialize Firebase
  initializeApp(aClient, app, getAuth(user_auth), processData, "🔐 authTask");
  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);
}

void sendDataToDB(){
   // Maintain authentication and async tasks
  app.loop();
  // Check if authentication is ready
      if (WiFi.status() != WL_CONNECTED)
    {
        return;
    }

    // Firebase authentication must be ready
    if (!app.ready())
    {
        return;
    }
  if (app.ready()){ 
    // Periodic data sending every 10 seconds
    unsigned long currentTime = millis();
    if (currentTime - lastSendTime >= sendInterval){
      // Update the last send time
      lastSendTime = currentTime;
  
      // send an int
  
      temperature= getTemp();
      humidity = getHumidity();
      setHumidity = receivedCommand.setHumidity;
      setTemperature = receivedCommand.setTemp;
      totalIncubationDays = receivedCommand.incubationDays;
      wifiStatus = 1;
      hatchingDays = receivedCommand.hatchingDays;
      currentIncubationDay = 5;


    object_t json;

    JsonWriter writer;

    object_t obj1;
    object_t obj2;
    object_t obj3;
    object_t obj4;
    object_t obj5;
    object_t obj6;
    object_t obj7;
    object_t obj8;

    writer.create(obj1, "temperature", temperature);
    writer.create(obj2, "humidity", humidity);
    writer.create(obj3, "setTemperature", setTemperature);
    writer.create(obj4, "setHumidity", setHumidity);

    writer.create(obj5, "totalIncubationDays", totalIncubationDays);
    writer.create(obj6, "hatchingDays", hatchingDays);
    writer.create(obj7, "currentIncubationDay", currentIncubationDay);
    writer.create(obj8, "wifiStatus", wifiCode);

    // Combine everything into one JSON object
    writer.join(
        json,
        8,
        obj1,
        obj2,
        obj3,
        obj4,
        obj5,
        obj6,
        obj7,
        obj8
    );

    // Optional: see exactly what is being sent
    Serial.print("Sending JSON: ");
    Serial.println(json);

    // Send ONE Firebase update
    Database.update<object_t>(
        aClient,
        "/test",
        json,
        processData,
        "incubatorData"
    );
    }
  }
}


void processData(AsyncResult &aResult) {
  if (!aResult.isResult())
    return;

  if (aResult.isEvent())
    Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.eventLog().message().c_str(), aResult.eventLog().code());

  if (aResult.isDebug())
    Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());

  if (aResult.isError())
    Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());

  if (aResult.available())
    Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
}