#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <Preferences.h>
#include "GUI.h"
#include "I2C.h"
Preferences prefs;
#define PREFS_NAMESPACE "incubator"
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

void save_incubation_params() {
  prefs.begin(PREFS_NAMESPACE, false); // false = read/write
  prefs.putFloat("setTemp", outCommand.setTemp);
  prefs.putFloat("setHumidity", outCommand.setHumidity);
  prefs.putUShort("incubDays", outCommand.incubationDays);
  prefs.putUShort("hatchDays", outCommand.hatchingDays);
  prefs.putULong("turnInterval", outCommand.turnInterval);
  prefs.putFloat("hatchHumidity", outCommand.hatchingHumidity);
  prefs.end();
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
//function for chicken
static void button_3_event_cb(lv_event_t *e) {
//   someVariable = 1; // or toggle, increment, whatever you need
outCommand.setTemp = 37.5;
outCommand.setHumidity = 62;
outCommand.incubationDays = 21 ;
outCommand.hatchingDays = 18;
outCommand.hatchingHumidity = 70;
outCommand.turnInterval = 60;
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
save_incubation_params();
}

//Function for GuineaFowl
static void button_4_event_cb(lv_event_t *e) {
//   someVariable = 1; // or toggle, increment, whatever you need
outCommand.setTemp = 37.5;
outCommand.setHumidity = 55;
outCommand.incubationDays = 28;
outCommand.hatchingDays = 25;
outCommand.hatchingHumidity = 70;
outCommand.turnInterval = 120;
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
save_incubation_params();
}

// Call back function for duck
static void button_5_event_cb(lv_event_t *e) {
//   someVariable = 1; // or toggle, increment, whatever you need
outCommand.setTemp = 37.5;
outCommand.setHumidity = 60;
outCommand.incubationDays = 28;
outCommand.hatchingDays = 25;
outCommand.hatchingHumidity = 70;
outCommand.turnInterval = 120;
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
save_incubation_params();
}
// call back for turkey
static void button_6_event_cb(lv_event_t *e) {
//   someVariable = 1; // or toggle, increment, whatever you need
outCommand.setTemp = 37.5;
outCommand.setHumidity = 60;
outCommand.incubationDays = 28;
outCommand.hatchingDays = 25;
outCommand.hatchingHumidity = 68;
outCommand.turnInterval = 120;
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

save_incubation_params();
}

static void start_incubation_custom_cb(lv_event_t *e) {
//   someVariable = 1; // or toggle, increment, whatever you need
// outCommand.setTemp = 37.5;
// outCommand.setHumidity = 60;
// outCommand.incubationDays = 28;
// outCommand.hatchingDays = 25;
// outCommand.hatchingHumidity = 68;
// outCommand.turnInterval = 120;
  outCommand.setTemp          = atof(lv_textarea_get_text(GUI_Textarea__Set_ParametersCustomMode__Textarea));
  outCommand.setHumidity      = atof(lv_textarea_get_text(GUI_Textarea__Set_ParametersCustomMode__Textarea_7));
  outCommand.incubationDays   = (uint16_t)atoi(lv_textarea_get_text(GUI_Textarea__Set_ParametersCustomMode__Textarea_1));
  outCommand.hatchingDays     = (uint16_t)atoi(lv_textarea_get_text(GUI_Textarea__Set_ParametersCustomMode__Textarea_8));
  outCommand.turnInterval     = (uint32_t)atol(lv_textarea_get_text(GUI_Textarea__Set_ParametersCustomMode__Textarea_2));
  outCommand.hatchingHumidity = atof(lv_textarea_get_text(GUI_Textarea__Set_ParametersCustomMode__Textarea_3));
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
save_incubation_params();
}
static void startAutoIncubation(lv_event_t *e) {
outCommand.startIncubation = 1;
outCommand.stopIncubation = 0;

  prefs.begin(PREFS_NAMESPACE, false);
  prefs.putBool("incubActive", true);
  prefs.end();
}

static void stopAutoIncubation(lv_event_t *e) {
outCommand.startIncubation = 0;
outCommand.stopIncubation = 1;
outCommand.fanStatusCYD = 0;
outCommand.hatchingDays= 0;
outCommand.hatchingHumidity= 0;
outCommand.heaterStatusCYD = 0;
outCommand.incubationDays = 0;
outCommand.motorStatusCYD= 0;
outCommand.setHumidity = 0.0;
outCommand.setTemp = 0.0;
  prefs.begin(PREFS_NAMESPACE, false);
  prefs.putFloat("setTemp", 0.0);
  prefs.putFloat("setHumidity", 0.0);
  prefs.putUShort("incubDays", 0);
  prefs.putUShort("hatchDays", 0);
  prefs.putULong("turnInterval", 0);
  prefs.putFloat("hatchHumidity", 0.0);
  prefs.putBool("incubActive", false);
  prefs.end();
}
void useCallback (){
        // Registering event callback functions
    lv_obj_add_event_cb(GUI_Button__MotorControl__Button_33, clockwise_button_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(GUI_Button__MotorControl__Button_34, anticlockwise_button_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(GUI_Switch__MotorControl__Switch_1, heater_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(GUI_Switch__MotorControl__Switch, collector_switch_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    // The event for the chicken button
    lv_obj_add_event_cb(GUI_Button__Set_ParametersAutoMode__Button_3, button_3_event_cb, LV_EVENT_CLICKED, NULL);
    // The event for guinea fowl button_3_event_cb
    lv_obj_add_event_cb(GUI_Button__Set_ParametersAutoMode__Button_4, button_4_event_cb, LV_EVENT_CLICKED, NULL);
    // The event for duck
     lv_obj_add_event_cb(GUI_Button__Set_ParametersAutoMode__Button_5, button_5_event_cb, LV_EVENT_CLICKED, NULL);
    // The event for turkey
    lv_obj_add_event_cb(GUI_Button__Set_ParametersAutoMode__Button_6, button_6_event_cb, LV_EVENT_CLICKED, NULL);
    // Start auto incubation
    lv_obj_add_event_cb(GUI_Button__AutoModePopUp__AutoModePopUp_StartButton, startAutoIncubation, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(GUI_Button__CustomModePopUp__CustomModePopUp_StartButton, startAutoIncubation, LV_EVENT_CLICKED, NULL);
    // Set the parameters whenever I click on the start incubation button
    lv_obj_add_event_cb(GUI_Button__Set_ParametersCustomMode__Button_24, start_incubation_custom_cb, LV_EVENT_CLICKED, NULL);
    //Stop Incubation
    lv_obj_add_event_cb(GUI_Button__Screen__Button_2, stopAutoIncubation, LV_EVENT_CLICKED, NULL);

}



void load_incubation_params() {
  prefs.begin(PREFS_NAMESPACE, true); // true = read-only
  outCommand.setTemp          = prefs.getFloat("setTemp", 37.5);      // 2nd arg = default if never saved before
  outCommand.setHumidity      = prefs.getFloat("setHumidity", 60.0);
  outCommand.incubationDays   = prefs.getUShort("incubDays", 21);
  outCommand.hatchingDays     = prefs.getUShort("hatchDays", 18);
  outCommand.turnInterval     = prefs.getULong("turnInterval", 20);
  outCommand.hatchingHumidity = prefs.getFloat("hatchHumidity", 70.0);


  if (prefs.getBool("incubActive", false)) {
    outCommand.startIncubation = 1;
    outCommand.stopIncubation = 0;
  }else{
      outCommand.startIncubation = 0;
    outCommand.stopIncubation = 1;
  }
  prefs.end();
}

// Set parameters on screenLoad if Incubation is still active

void setParametersIfIncubationActive(){
char tempLabel[8];
char humidityLabel[8];
char turnIntervalLabel[8];
char hatchingDaysLabel[8];
char incubationDaysLabel[8];
char humidityHatchingLabel[8];
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