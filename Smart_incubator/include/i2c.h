#ifndef I2C_H
#define I2C_H

#include <Arduino.h>
#include <Wire.h>

#define CYD_ADDR 0x08
#define SDA_PIN 21
#define SCL_PIN 22

#pragma pack(push, 1)

// ================================
// ESP32 → CYD
// ================================
struct SensorData
{
    float temperature;
    float humidity;

    uint16_t daysLeft;
    uint8_t hour;
    uint8_t minutes;
    uint8_t wifiStatus;
    uint8_t motorStatus;
    uint8_t heaterStatus;
};

// ================================
// CYD → ESP32
// ================================
struct CommandPacket
{
    uint8_t stopIncubation;
    uint8_t startIncubation;

    float setTemp;
    float setHumidity;

    uint16_t incubationDays;
    uint16_t hatchingDays;

    uint32_t turnInterval;

    uint8_t heaterStatusCYD;
    uint8_t motorStatusCYD;
    uint8_t fanStatusCYD;

    float hatchingHumidity;

    char SSID[33];
    char Password[64];
};

#pragma pack(pop)

extern CommandPacket receivedCommand;
// extern uint8_t hatchingDay;
// extern float hatchingHumidity;
// extern uint8_t stopIncubation;
// extern uint8_t startIncubation;
 extern    float setTemp;
extern    float setHumidity;
// extern     uint16_t incubationDays;

// extern    uint32_t turnInterval;

// extern    uint8_t heaterStatusCYD;
// extern   uint8_t motorStatusCYD;
// extern    uint8_t fanStatusCYD;
// extern     char SSID[33];
// extern   char Password[64];
void initializePacket();
void sendSensorData();
void receiveCommandFromCYD();

#endif
