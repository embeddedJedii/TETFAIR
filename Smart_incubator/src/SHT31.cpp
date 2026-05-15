#include <Arduino.h>
#include <wire.h>
#include <Adafruit_SHT31.h>
#include "sht31.h"
Adafruit_SHT31 sht31 = Adafruit_SHT31();

void shtStatus() {
if (!sht31.begin(0x44)) {
    Serial.println("ERROR: SHT31 not found! Check wiring.");
    while (1) delay(1);
  }
}

float getTemperature() {
  float temp = sht31.readTemperature();
   if (!isnan(temp)) {
    Serial.printf("Temp: %.1f °C\n", temp);
  } else {
    Serial.println("ERROR: Failed to read sensor!");
  }
  return temp;
}

float getHumidity() {
  float humidity = sht31.readHumidity();
  if (!isnan(humidity)) {
    Serial.printf("Humidity: %.1f %%\n", humidity);
  } else {
    Serial.println("ERROR: Failed to read sensor!");
  }
  return humidity;
}

