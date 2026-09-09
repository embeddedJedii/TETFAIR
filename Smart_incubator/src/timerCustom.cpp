#include <Arduino.h>
#include <RTClib.h>
#include <WiFi.h>
#include <Preferences.h>
#include "I2Cinit.h"
#include "time.h"
#include "turningSystem.h"
#include "temperature.h"
const char* ssid = "ICT"; //Received from CYD
const char* password = "INNOV8HUB";
const long gmtOffset_sec = 3600;      // WAT = UTC+1
const int daylightOffset_sec = 0;     // Nigeria doesn't use DST
RTC_DS3231 rtc;   
Preferences preferences;
int TOTAL_INCUBATION_DAYS = 21; //Change this accordingly after integrating the I2C communication with the CYD

bool incubationActive = false; //This should be gotten from the CYD

uint32_t incubationStartTime = 0; 
uint8_t circulationFan = 33;
int incubationDay = 0;

uint32_t elapsedSeconds = 0;
void ciculationFanInit (){
  pinMode(circulationFan, OUTPUT);
  digitalWrite(circulationFan, LOW); // Turn off circulation fan initially
}
void syncRTCviaNTP() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");

  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    rtc.adjust(DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                         timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
    Serial.println("RTC synced via NTP");
  } else {
    Serial.println("Failed to get NTP time");
  }
} 
void RTCInit (){
  
  if (!rtc.begin(&I2Cbus2)) {
    Serial.println("RTC not found on bus 2 (18/19)");
  } else {
    Serial.println("RTC OK on bus 2 (18/19)");
    if (rtc.lostPower()) {
      // Sets RTC to compile time if it lost power — remove after first flash
    //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
           if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("Synchronizing RTC via NTP...");
            syncRTCviaNTP();
        }
        else
        {
            Serial.println("WiFi not connected. Cannot sync RTC via NTP.");
            Serial.println("RTC time may be incorrect.");
        }
     
    }
  }
}

void startIncubation()
{
    DateTime now = rtc.now();
    lastTurnTime = now.unixtime(); // Initialize lastTurnTime to the current time when incubation starts
    incubationStartTime = now.unixtime();
    incubationActive = true;

    preferences.putBool("active", true);
    preferences.putUInt("start", incubationStartTime);

    Serial.println();
    Serial.println("================================");
    Serial.println("INCUBATION STARTED");
    Serial.println("================================");

    Serial.print("Start timestamp: ");
    Serial.println(incubationStartTime);

    // Read it back immediately
    uint32_t savedStart =
        preferences.getUInt("start", 0);

    Serial.print("NVS saved timestamp: ");
    Serial.println(savedStart);
    // tURN ON CIRCULATION FAN WHEN INCUBATION STARTS
    digitalWrite(circulationFan, HIGH); // Turn on circulation fan
}
// void startIncubation()
// {
//     DateTime now = rtc.now();

//     // Convert RTC date/time to Unix timestamp
//     incubationStartTime = now.unixtime();

//     incubationActive = true;

//     // Save to ESP32 NVS
//     preferences.putBool("active", true);
//     preferences.putUInt("start", incubationStartTime);

//     Serial.println();
//     Serial.println("================================");
//     Serial.println("INCUBATION STARTED");
//     Serial.println("================================");

//     Serial.print("Start time: ");
//     Serial.print(now.year());
//     Serial.print("-");
//     Serial.print(now.month());
//     Serial.print("-");
//     Serial.print(now.day());

//     Serial.print(" ");

//     Serial.print(now.hour());
//     Serial.print(":");
//     Serial.print(now.minute());
//     Serial.print(":");
//     Serial.println(now.second());
// }

void stopIncubation()
{
    incubationActive = false;

    incubationStartTime = 0;

    incubationDay = 0;

    elapsedSeconds = 0;

    // Save inactive state
    preferences.putBool("active", false);
    preferences.putUInt("start", 0);
    Serial.println();
    Serial.println("================================");
    Serial.println("INCUBATION STOPPED");
    Serial.println("================================");
}

// void loadIncubation()
// {
//     incubationActive =
//         preferences.getBool("active", false);

//     incubationStartTime =
//         preferences.getUInt("start", 0);


//     if (incubationActive && incubationStartTime != 0)
//     {
//         Serial.println();
//         Serial.println("Saved incubation found.");

//         DateTime startTime(incubationStartTime);

//         Serial.print("Started: ");

//         Serial.print(startTime.year());
//         Serial.print("-");
//         Serial.print(startTime.month());
//         Serial.print("-");
//         Serial.print(startTime.day());

//         Serial.print(" ");

//         Serial.print(startTime.hour());
//         Serial.print(":");
//         Serial.print(startTime.minute());
//         Serial.print(":");
//         Serial.println(startTime.second());
//     }
//     else
//     {
//         Serial.println("No active incubation.");
//     }
// }
void loadIncubation()
{
    incubationActive =
        preferences.getBool("active", false);

    incubationStartTime =
        preferences.getUInt("start", 0);
    
    DateTime now = rtc.now();
    lastTurnTime = now.unixtime();

    Serial.println();
    Serial.println("========== INCUBATION LOAD ==========");

    Serial.print("Active: ");
    Serial.println(incubationActive);

    Serial.print("Saved start timestamp: ");
    Serial.println(incubationStartTime);

    Serial.print("Current RTC timestamp: ");
    Serial.println(rtc.now().unixtime());

    Serial.println("=====================================");
}
void updateIncubation()
{
    if (!incubationActive)
        return;


    DateTime now = rtc.now();


    // Calculate elapsed seconds
    elapsedSeconds =
        now.unixtime() - incubationStartTime;


    // Calculate incubation day
    //
    // First 24 hours = Day 1
    // 24-48 hours   = Day 2
    // etc.

    incubationDay =
        (elapsedSeconds / 86400) + 1;


    // Prevent going beyond final day

    if (incubationDay > TOTAL_INCUBATION_DAYS)
    {
        incubationDay =
            TOTAL_INCUBATION_DAYS;
    }
}

void printIncubationStatus()
{
    if (!incubationActive)
    {
        Serial.println("Incubation inactive.");
        return;
    }


    DateTime now = rtc.now();

    uint32_t days =
        elapsedSeconds / 86400;

    uint32_t hours =
        (elapsedSeconds % 86400) / 3600;

    uint32_t minutes =
        (elapsedSeconds % 3600) / 60;

    uint32_t seconds =
        elapsedSeconds % 60;

    DateTime startTime(incubationStartTime);
    Serial.println();
    Serial.println("------------- INCUBATOR -------------");


    Serial.print("Current time: ");

    Serial.print(now.year());
    Serial.print("-");
    Serial.print(now.month());
    Serial.print("-");
    Serial.print(now.day());

    Serial.print(" ");
 
    Serial.print(now.hour());
    Serial.print(":");
    Serial.print(now.minute());
    Serial.print(":");
    Serial.println(now.second());


    Serial.print("Incubation day: ");
    Serial.print(incubationDay);

    Serial.print(" / ");
    Serial.println(TOTAL_INCUBATION_DAYS);


    Serial.print("Elapsed: ");

    Serial.print(days);
    Serial.print(" days ");

    Serial.print(hours);
    Serial.print(" hours ");

    Serial.print(minutes);
    Serial.print(" minutes ");

    Serial.print(seconds);
    Serial.println(" seconds");


    Serial.println("-------------------------------------");
}

void printUpdateIncubationStatus(){
    static unsigned long lastUpdate = 0;


    // Update once every minute (60000 milliseconds)

    if (millis() - lastUpdate >= 1000)
    {
        lastUpdate = millis();


        // Update incubation calculations

        updateIncubation();


        // Print status every 10 seconds

        static int counter = 0;

        counter++;

        if (counter >= 10)
        {
            counter = 0;

            printIncubationStatus();
        }
    }
}

uint16_t sendIncubationDays(){
  if (!incubationActive)
    {
        Serial.println("Incubation inactive.");
        return 0;
    }


    DateTime now = rtc.now();

    uint16_t days =
        elapsedSeconds / 86400;
    return days;
}

// void eggTurningControl(){
//   int lastTurnTime = 0;
//   const uint32_t TURN_INTERVAL = 4UL * 60UL * 60UL;  // 4 hours // CHANGE THIS ACCORDINGLY
//  if(incubationActive){
//   if(incubationDay >=1 && incubationDay <= (TOTAL_INCUBATION_DAYS -3)){
    
//     if((millis() - lastTurnTime) >= TURN_INTERVAL){ // 4 hours in milliseconds
//       Serial.println("Turning eggs now...");
//       // Add your egg turning logic here
//       lastTurnTime = 0;  // Update the last turn time
//     } 
//   }
//  }
// }

// void eggTurningControl()
// {
//     static uint32_t lastTurnTime = 0;

//     const uint32_t TURN_INTERVAL = 4UL * 60UL * 60UL;  // 4 hours = 14,400 seconds (4*60*60)

//     if (incubationActive)
//     {
//         if (incubationDay >= 1 &&
//             incubationDay <= (TOTAL_INCUBATION_DAYS - 3))
//         {
//             uint32_t currentTime = rtc.now().unixtime();

//             if (currentTime - lastTurnTime >= TURN_INTERVAL)
//             {
//                 Serial.println("Turning eggs now...");
//               //  Egg turning logic
//                 turningLogic();
//                 lastTurnTime = currentTime;
//             }
//         }
//     }
// }

