#ifndef WIFIUPDATE_H
#define WIFIUPDATE_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern char selectedSSID[33];

void wifi_list_setup();
void wifi_list_loop();

#ifdef __cplusplus
}
#endif

#endif // WIFIUPDATE_H