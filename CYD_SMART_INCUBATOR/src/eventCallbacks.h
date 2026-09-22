#ifndef EVENTCALLBACKS_H
#define EVENTCALLBACKS_H

#include <Arduino.h>
#include <esp32_smartdisplay.h>

void clockwise_button_event_cb(lv_event_t *e);
void anticlockwise_button_event_cb(lv_event_t *e);
void heater_switch_event_cb(lv_event_t *e);
void collector_switch_event_cb(lv_event_t *e);
void button_3_event_cb(lv_event_t *e);
void button_4_event_cb(lv_event_t *e);
void button_5_event_cb(lv_event_t *e);
void button_6_event_cb(lv_event_t *e);
void startAutoIncubation(lv_event_t *e);
void useCallback();
void save_incubation_params();
void load_incubation_params();
void setParametersIfIncubationActive();
#endif
