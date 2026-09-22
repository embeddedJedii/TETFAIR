#include <Arduino.h>
#include <Wire.h>
// smartdisplay_init() already calls lv_init() and sets up the display + touch
// drivers for the ESP32-8048S070C, so we call GUI_init() (not GUI_load()) —
// GUI_init() only builds the screens/theme/animations and loads the first one,
// skipping GUI's own (unimplemented) lv_init()/HAL_init() path.

uint8_t motorStatus = 0;
uint8_t heaterStatus = 0; // 0 = off, 1 = on
uint8_t collectorFanStatus = 0;
#define SLAVE_ADDR 0x08

#define SDA_PIN 17
#define SCL_PIN 18


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


// =====================================================
// I2C
// =====================================================

TwoWire I2C_EXT = TwoWire(1);


// =====================================================
// RECEIVED DATA FROM ESP32
// =====================================================

volatile SensorData lastReceived = {
    0.0,
    0.0,
    0,
    0,
    0,
    1,
    0,
    0
};


// =====================================================
// COMMAND DATA TO SEND TO ESP32
// =====================================================

CommandPacket outCommand =
{
    0,       // stopIncubation
    0,       // startIncubation

    37.5,    // setTemp
    60.0,    // setHumidity

    21,      // incubationDays
    18,      // hatchingDays

    20,      // turnInterval (20 seconds for testing)
    0,// heaterStatus,
    0, //motorStatus,
    0, //fanStatusCYD
    70.0,    // hatchingHumidity

    "ICT",
    "INNOV8HUB"
};


// =====================================================
// I2C RECEIVE CALLBACK
// =====================================================

void onReceiveHandler(int numBytes)
{
    if (numBytes == sizeof(SensorData))
    {
        SensorData incoming;

        I2C_EXT.readBytes(
            (uint8_t*)&incoming,
            sizeof(incoming)
        );

        memcpy(
            (void*)&lastReceived,
            &incoming,
            sizeof(SensorData)
        );
    }
    else
    {
        // Clear unexpected data
        while (I2C_EXT.available())
        {
            I2C_EXT.read();
        }
    }
}


// =====================================================
// I2C REQUEST CALLBACK
// =====================================================

// void onRequestHandler()
// {
//     I2C_EXT.write(
//         (uint8_t*)&outCommand,
//         sizeof(outCommand)
//     );
// }

void onRequestHandler()
{
    I2C_EXT.write(
        (uint8_t*)&outCommand,
        sizeof(CommandPacket)
    );
} 
