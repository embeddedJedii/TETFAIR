#ifndef I2C_H
#define I2C_H

#include <Arduino.h>
#include <Wire.h>

#define SLAVE_ADDR 0x08
#define SDA_PIN 17
#define SCL_PIN 18

#pragma pack(push, 1)

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

extern TwoWire I2C_EXT;
extern volatile SensorData lastReceived;
extern CommandPacket outCommand;
extern uint8_t motorStatus;
extern uint8_t heaterStatus;
extern uint8_t collectorFanStatus;

void onReceiveHandler(int numBytes);
void onRequestHandler();

#endif
