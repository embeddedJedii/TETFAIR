#include <Arduino.h>
#include <Preferences.h>
#include "temperature.h"
#include "timerCustom.h"
#include "buzzer.h"
#include "turningSystem.h"
#include "i2c.h"
#include "internet.h"
void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
 tempHumidInit();
 RTCInit();
 buzzerInit();
 tempPinInit();
 ciculationFanInit();
    preferences.begin(
        "incubator",
        false
    ); 
    loadIncubation();
    limitSwitchInit();
    motorPinsInit();
    humidifierInit();  
    initializePacket();
    
    initializeFirebase();
}

void loop() {
  //put your main code here, to run repeatedly:
  isConnected();
//   connectToWifi ();
   sendDataToDB();
//syncRTCviaNTP();
  float temperature = getTemp(); 
  float humidity = getHumidity();
  printIncubationStatus();
  printUpdateIncubationStatus();
  eggTurningControl();
  heaterLogic();
  hatchingAlgorithm();
  checkTempHumFault();
  checkTempHumOvershoot();
  sendSensorData();
//   delay(100);
  receiveCommandFromCYD();
//   delay(100); //900
//  updateValues();
 //The following should be replaced with the start and stop command from the CYD
// if(!receivedCommand.startIncubation) buzzerOff();
     if (Serial.available())
    {
        char command =
            Serial.read();


        if (command == 'S' ||  
            command == 's')
        {
            startIncubation();  
        }


        if (command == 'X' ||
            command == 'x')
        {
            stopIncubation();
        }
    }

    Serial.println("Receiveeeeeeeee");
    Serial.println(receivedCommand.hatchingDays);
//   delay(100);
}

 