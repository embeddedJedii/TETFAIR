#ifndef SHT31_H
#define SHT31_H

#include <Arduino.h>
#include <Adafruit_SHT31.h>

extern Adafruit_SHT31 sht31;

// Initialize the SHT31 sensor. Returns true on success.
bool initSHT31(uint8_t i2caddr = SHT31_DEFAULT_ADDR);

// Read temperature in degrees Celsius. Returns NAN on error.
float getTemperature();

// Read relative humidity in percent. Returns NAN on error.
float getHumidity();

// Check SHT31 sensor status and initialize if needed.
extern void shtStatus();

#endif 

