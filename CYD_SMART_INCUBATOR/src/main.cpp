#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <Wire.h>
#include "GUI.h"

// smartdisplay_init() already calls lv_init() and sets up the display + touch
// drivers for the ESP32-8048S070C, so we call GUI_init() (not GUI_load()) —
// GUI_init() only builds the screens/theme/animations and loads the first one,
// skipping GUI's own (unimplemented) lv_init()/HAL_init() path.

static uint32_t lv_last_tick = 0;
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

// Motor_callback

    //clockwise motor
static void clockwise_button_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_PRESSED) {
    motorStatus = 1;
  } else if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST) {
    // PRESS_LOST fires if the finger slides off the button while still down
    // (e.g. dragged off the edge) -- without handling it too, clockwiseStatus
    // could get stuck at 1 with the motor still "on" even though nothing is
    // touching the button anymore.
    motorStatus = 0;
  }
}

    //Anti clockwise motor
static void anticlockwise_button_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_PRESSED) {
    motorStatus = 2;
  } else if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST) {
    // PRESS_LOST fires if the finger slides off the button while still down
    // (e.g. dragged off the edge) -- without handling it too, clockwiseStatus
    // could get stuck at 1 with the motor still "on" even though nothing is
    // touching the button anymore.
    motorStatus = 0;
  }


}  

// Heater callback


static void heater_switch_event_cb(lv_event_t *e) {
  lv_obj_t *sw = (lv_obj_t *)lv_event_get_target(e);
  heaterStatus = lv_obj_has_state(sw, LV_STATE_CHECKED) ? 1 : 0;
}

//Collector fan callback
static void collector_switch_event_cb(lv_event_t *e) {
  lv_obj_t *sw = (lv_obj_t *)lv_event_get_target(e);
  collectorFanStatus = lv_obj_has_state(sw, LV_STATE_CHECKED) ? 1 : 0;
}

//Automode callback functions

static void button_3_event_cb(lv_event_t *e) {
//   someVariable = 1; // or toggle, increment, whatever you need
outCommand.setTemp = 37.5;
outCommand.setHumidity = 62;
outCommand.incubationDays = 21 ;
outCommand.hatchingDays = 18;
outCommand.hatchingHumidity = 70;
outCommand.turnInterval = 45;
char tempLabel[8];
char humidityLabel[8];
char turnIntervalLabel[8];
char hatchingDaysLabel[8];
char incubationDaysLabel[8];
char humidityHatchingLabel[8];
// for set temperature
snprintf(tempLabel, sizeof(tempLabel), "%.1f", outCommand.setTemp);
lv_label_set_text(GUI_Label__Set_ParametersAutoMode__Label_4, tempLabel);
//For set humidity
snprintf(humidityLabel, sizeof(humidityLabel), "%.1f", outCommand.setHumidity);
lv_label_set_text(GUI_Label__Set_ParametersAutoMode__Label_18, humidityLabel);
// For turn Interval
snprintf(turnIntervalLabel, sizeof(turnIntervalLabel), "%d", outCommand.turnInterval);
lv_label_set_text(GUI_Label__Set_ParametersAutoMode__Label_20, turnIntervalLabel);
// for hatching days
snprintf(hatchingDaysLabel, sizeof(hatchingDaysLabel), "%d", outCommand.hatchingDays);
lv_label_set_text(GUI_Label__Set_ParametersAutoMode__Label_17, hatchingDaysLabel);
// For Incubation days
snprintf(incubationDaysLabel, sizeof(incubationDaysLabel), "%d", outCommand.incubationDays);
lv_label_set_text(GUI_Label__Set_ParametersAutoMode__Label_19, incubationDaysLabel);

// for humidity at hatching days
snprintf(humidityHatchingLabel, sizeof(humidityHatchingLabel), "%.1f", outCommand.hatchingHumidity);
lv_label_set_text(GUI_Label__Set_ParametersAutoMode__Label_6, humidityHatchingLabel);
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

    // Registering event callback functions
    lv_obj_add_event_cb(GUI_Button__MotorControl__Button_33, clockwise_button_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(GUI_Button__MotorControl__Button_34, anticlockwise_button_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(GUI_Switch__MotorControl__Switch_1, heater_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(GUI_Switch__MotorControl__Switch, collector_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    // The event for the chicken button
    lv_obj_add_event_cb(GUI_Button__Set_ParametersAutoMode__Button_3, button_3_event_cb, LV_EVENT_CLICKED, NULL);
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
    outCommand.heaterStatusCYD = heaterStatus;
    outCommand.motorStatusCYD = motorStatus;
    outCommand.fanStatusCYD = collectorFanStatus;
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

    lv_obj_t *wifi_icons[] = {
    GUI_Image__Home__HOME_WiFi_image,
    GUI_Image__Screen__HOME_WiFi_image_1,
    GUI_Image__MotorControl__MotorControl_Wifi,
    GUI_Image__Set_ParametersAutoMode__AutoMode_WIFI_image,
    GUI_Image__Set_ParametersCustomMode__CustomMode_WIFI,
    GUI_Image__Available_Networks__AutoMode_WIFI_image_1,
    GUI_Image__AutoModePopUp__image_46,
    GUI_Image__CustomModePopUp__image_48,
    GUI_Image__PaswordInputPopUp__AutoMode_WIFI_image_2,
  };

  for (lv_obj_t *icon : wifi_icons) {
    if (snapshot.wifiStatus == 1) {
      lv_obj_remove_flag(icon, LV_OBJ_FLAG_HIDDEN);
    } else {
      lv_obj_add_flag(icon, LV_OBJ_FLAG_HIDDEN);
    }
  }
// float temperature = 47.5;

// char tempText[10];

float temperature = snapshot.temperature;
float humidity = snapshot.humidity;
uint8_t daysLeft = snapshot.daysLeft;
uint8_t hour = snapshot.hour;
uint8_t minutes = snapshot.minutes;
char tempText[8];
char humText[8];
char daysLeftText[8];
char hourText[8];
snprintf(hourText, sizeof(hourText), "%02d:%02d", hour, minutes);
snprintf(tempText, sizeof(tempText), "%.1f", temperature);
snprintf(humText, sizeof(humText), " %.1f", humidity);
snprintf(daysLeftText, sizeof(daysLeftText), " %d", daysLeft);
lv_label_set_text(GUI_Label__Home__Label_7, tempText);
lv_label_set_text(GUI_Label__Home__Room_Humidity,humText);
lv_label_set_text(GUI_Label__Home__Label_8,daysLeftText);
lv_label_set_text(GUI_Label__Home__HOME_TIMEimage,hourText);
lv_label_set_text(GUI_Label__Available_Networks__AutoMode_TIME_1,hourText);

lv_label_set_text(GUI_Label__AutoModePopUp__label_55,hourText);
lv_label_set_text(GUI_Label__CustomModePopUp__label_100,hourText);
lv_label_set_text(GUI_Label__MotorControl__MotorControl_Time,hourText);
lv_label_set_text(GUI_Label__PaswordInputPopUp__AutoMode_TIME_2,hourText);
lv_label_set_text(GUI_Label__Screen__HOME_TIMEimage_2,hourText);
lv_label_set_text(GUI_Label__Set_ParametersAutoMode__AutoMode_TIME,hourText);
lv_label_set_text(GUI_Label__Set_ParametersCustomMode__CustomMode_TIME,hourText);

if(!snapshot.temperature || !snapshot.humidity){
        lv_label_set_text(
    GUI_Label__Home__SystemHealthDiagnostic_1,
    "FAULT!!!"
);
    lv_label_set_text(
   GUI_Label__Home__SystemHealthDiagnostic_2,
    "Temperature sensor fault"
);
lv_obj_set_style_text_color(
    GUI_Label__Home__SystemHealthDiagnostic_1,
    lv_color_hex(0xFF0000),
    LV_PART_MAIN
);
lv_obj_set_style_text_color(
    GUI_Label__Home__SystemHealthDiagnostic_2,
    lv_color_hex(0xFF0000),
    LV_PART_MAIN
);
}else{
            lv_label_set_text(
    GUI_Label__Home__SystemHealthDiagnostic_1,
    "OPERATIONAL"
);
    lv_label_set_text(
   GUI_Label__Home__SystemHealthDiagnostic_2,
    "All systems are normal"
);
lv_obj_set_style_text_color(
    GUI_Label__Home__SystemHealthDiagnostic_1,
   lv_color_hex(0x00FF00),
    LV_PART_MAIN
);
lv_obj_set_style_text_color(
    GUI_Label__Home__SystemHealthDiagnostic_2, 
   lv_color_hex(0x00FF00),
    LV_PART_MAIN
);
}

if(!snapshot.heaterStatus){
    lv_label_set_text(
    GUI_Label__Home__Heater_Diagnostic_1,
    "IDLE"
);
}else{
        lv_label_set_text(
    GUI_Label__Home__Heater_Diagnostic_1,
    "OPERATIONAL"
);
}
if(!snapshot.motorStatus){
    lv_label_set_text(
   GUI_Label__Home__Motor_Sensor_Diagnostic_1,
    "IDLE"
);
}else{
            lv_label_set_text(
   GUI_Label__Home__Motor_Sensor_Diagnostic_1,
    "OPERATIONAL"
);
}
//Manipulation the nmotor screen
// CLockwise button
 // 0 = released, 1 = pressed/held down



  lv_timer_handler();
  delay(5);
}
