#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include "GUI.h"
#include "I2C.h"
#include "eventCallbacks.h"
#include "wifiUpdate.h"
static uint32_t lv_last_tick = 0;

// Motor_callback

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
    lv_obj_add_flag(GUI_Button__Available_Networks__AvailableNetworks1, LV_OBJ_FLAG_HIDDEN);
lv_obj_set_flex_flow(GUI_Container__Available_Networks__container_8, LV_FLEX_FLOW_COLUMN);
lv_obj_add_flag(GUI_Container__Available_Networks__container_8, LV_OBJ_FLAG_SCROLLABLE);
lv_obj_set_scroll_dir(GUI_Container__Available_Networks__container_8, LV_DIR_VER);
wifi_list_setup();
    useCallback();
    load_incubation_params();
    setParametersIfIncubationActive();
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
wifi_list_loop(); 

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

if(outCommand.stopIncubation){
    setParametersIfIncubationActive();
}

  lv_timer_handler();
  delay(5);
}

