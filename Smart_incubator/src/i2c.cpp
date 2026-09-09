#include <Arduino.h>
#include <Wire.h>
#include "temperature.h"
#include "i2c.h"

// Received command from CYD
CommandPacket receivedCommand;

// This should be in void setup() in main.cpp
void initializePacket(){
     // Initialize command packet
    memset(&receivedCommand, 0, sizeof(receivedCommand));

    Serial.print("Command packet size: ");
    Serial.println(sizeof(CommandPacket));

    Serial.print("Sensor packet size: ");
    Serial.println(sizeof(SensorData));
}

void sendSensorData()
{
    float t = getTemp();
    float h = getHumidity();

    if (isnan(t) || isnan(h))
    {
        Serial.println("Failed to read SHT31");
        t = 0.00;
        h = 0.00;
    }


    SensorData outPacket;

    outPacket.temperature = t; // Replace with actual temperature reading
    outPacket.humidity = h;

    // ----------------------------------
    // Replace these with your actual
    // incubation variables
    // ----------------------------------

    outPacket.daysLeft = 10;
    outPacket.hour = 14;
    outPacket.minutes = 35;
    outPacket.wifiStatus = 2;

    // ----------------------------------
    // Send packet to CYD
    // ----------------------------------

    Wire.beginTransmission(CYD_ADDR);

    Wire.write(
        (uint8_t*)&outPacket,
        sizeof(outPacket)
    );

    uint8_t error = Wire.endTransmission();

    if (error == 0)
    {
        Serial.printf(
            "Sent: %.2f C, %.2f %%RH, %d days, %02d:%02d\n",
            outPacket.temperature,
            outPacket.humidity,
            outPacket.daysLeft,
            outPacket.hour,
            outPacket.minutes
        );
    }
    else
    {
        Serial.print("I2C transmission error: ");
        Serial.println(error);
    }
}

void receiveCommandFromCYD()
{
    CommandPacket incoming;

    uint8_t requestedBytes = sizeof(CommandPacket);

    uint8_t receivedBytes = Wire.requestFrom(
        CYD_ADDR,
        requestedBytes
    );

    if (receivedBytes == sizeof(CommandPacket))
    {
        Wire.readBytes(
            (uint8_t*)&incoming,
            sizeof(incoming)
        );

        // Make sure strings are terminated
        incoming.SSID[sizeof(incoming.SSID) - 1] = '\0';
        incoming.Password[sizeof(incoming.Password) - 1] = '\0';

        memcpy(
            &receivedCommand,
            &incoming,
            sizeof(CommandPacket)
        );


        Serial.println();
        Serial.println("===== COMMAND FROM CYD =====");

        Serial.print("Stop incubation: ");
        Serial.println(receivedCommand.stopIncubation);

        Serial.print("Start incubation: ");
        Serial.println(receivedCommand.startIncubation);

        Serial.print("Set temperature: ");
        Serial.println(receivedCommand.setTemp);

        Serial.print("Set humidity: ");
        Serial.println(receivedCommand.setHumidity);

        Serial.print("Incubation days: ");
        Serial.println(receivedCommand.incubationDays);

        Serial.print("Hatching days: ");
        Serial.println(receivedCommand.hatchingDays);

        Serial.print("Turn interval: ");
        Serial.println(receivedCommand.turnInterval);

        Serial.print("Hatching humidity: ");
        Serial.println(receivedCommand.hatchingHumidity);

        Serial.print("SSID: ");
        Serial.println(receivedCommand.SSID);

        Serial.print("Password: ");
        Serial.println(receivedCommand.Password);

        Serial.println("============================");
    }
    else
    {
        // Clear any incomplete data
        while (Wire.available())
        {
            Wire.read();
        }

        Serial.print("Invalid command packet size. Received: ");
        Serial.println(receivedBytes);
    }
}