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
    connectToWifi ();
}

void loop() {
  // put your main code here, to run repeatedly:
  isConnected();
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
  delay(100);
  receiveCommandFromCYD();
  delay(900);
//   Serial.println("The last turn time is:");
//   Serial.println(lastTurnTime);
//   Serial.println("The incubation active status is:");
//   Serial.println(incubationActive);
//   Serial.println("The incubation day is:");
//   Serial.println(incubationDay);
//   Serial.println("The hatching day is:"); 
//   Serial.println(hatchingDay);
//   Serial.println("Motor turning status:");
// Serial.println(motorTurning);
//syncRTCviaNTP();
//   motorAction();
 //The following should be replaced with the start and stop command from the CYD

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
  delay(2000);
}

 