#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <Wire.h>
#include "GUI.h"

// smartdisplay_init() already calls lv_init() and sets up the display + touch
// drivers for the ESP32-8048S070C, so we call GUI_init() (not GUI_load()) —
// GUI_init() only builds the screens/theme/animations and loads the first one,
// skipping GUI's own (unimplemented) lv_init()/HAL_init() path.

static uint32_t lv_last_tick = 0;


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
void setup() {
  Serial.begin(115200);

  smartdisplay_init();

  // Uncomment if your panel is physically rotated relative to how you want
  // the UI to render:
  // auto display = lv_display_get_default();
  // lv_display_set_rotation(display, LV_DISPLAY_ROTATION_90);

  GUI_init(); // builds all screens, shows the splash screen first (see gui_core.c)
    I2C_EXT.begin(
        SLAVE_ADDR,
        SDA_PIN,
        SCL_PIN,
        100000
    );

    I2C_EXT.onReceive(onReceiveHandler);
    I2C_EXT.onRequest(onRequestHandler);


    Serial.println("CYD I2C slave ready");

    Serial.print("Command packet size: ");
    Serial.println(sizeof(CommandPacket));

    Serial.print("Sensor packet size: ");
    Serial.println(sizeof(SensorData));
  lv_last_tick = millis();
}

void loop() {
  const uint32_t now = millis();
  lv_tick_inc(now - lv_last_tick);
  lv_last_tick = now;
  SensorData snapshot;
     memcpy(
        &snapshot,
        (void*)&lastReceived,
        sizeof(SensorData)
    );

    if(snapshot.wifiStatus) Serial.println("Wifi is connected");
    else Serial.println("Wifi is not connected");
    Serial.printf(
        "Latest: %.2f C, %.2f %%RH, %d days, %02d:%02d, wifiStatus %d\n", 
        snapshot.temperature,
        snapshot.humidity,
        snapshot.daysLeft,
        snapshot.hour,
        snapshot.minutes,
        snapshot.wifiStatus
    );
float temperature = 47.5;

lv_label_set_text_fmt(
    GUI_Label__Home__Label_7,
    "%.2f",
    temperature
);
  lv_timer_handler();
  delay(5);
}
