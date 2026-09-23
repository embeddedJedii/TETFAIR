#include <arduino.h>
#include <esp32_smartdisplay.h>
#include "GUI.h"
// #include <WiFi.h>

// void scanWifi() {
//   Serial.println("Scanning for WiFi networks...");

//   int n = WiFi.scanNetworks();

//   if (n == 0) {
//     Serial.println("No networks found.");
//   } else {
//     String seenSSIDs[50];   // adjust size if you expect more networks
//     int seenCount = 0;



//     for (int i = 0; i < n; i++) {
//       String currentSSID = WiFi.SSID(i);

//       // Skip empty SSIDs (hidden networks)
//       if (currentSSID.length() == 0) continue;

//       // Check if this SSID has already been printed
//       bool alreadySeen = false;
//       for (int j = 0; j < seenCount; j++) {
//         if (seenSSIDs[j] == currentSSID) {  
//           alreadySeen = true;
//           break;
//         }
//       }

//       if (alreadySeen) continue;

//       // Mark as seen
//       seenSSIDs[seenCount] = currentSSID;
//       seenCount++;

//       // Print SSID and password status
//       Serial.print(currentSSID);
//       Serial.print(" - ");

//       if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) {
//         Serial.println("No Password");
//       } else {
//         Serial.println("Has Password");
//       }
//     }
//   }

//   Serial.println();
// }

// #include <WiFi.h>

// #define MAX_WIFI_ROWS 20
// static lv_obj_t *wifi_row_objs[MAX_WIFI_ROWS];
// static char *wifi_row_ssids[MAX_WIFI_ROWS]; // tracked so we can free() them
// static int wifi_row_count = 0;
// static bool wifi_scan_in_progress = false;

// char selectedSSID[33] = "";

// static void clear_wifi_list() {
//     for (int i = 0; i < wifi_row_count; i++) {
//         lv_obj_delete(wifi_row_objs[i]);
//         free(wifi_row_ssids[i]);
//         wifi_row_objs[i] = NULL;
//         wifi_row_ssids[i] = NULL;
//     }
//     wifi_row_count = 0;
// }

// static void wifi_row_clicked_event_cb(lv_event_t *e) {
//     const char *ssid = (const char *)lv_event_get_user_data(e);
//     strncpy(selectedSSID, ssid, sizeof(selectedSSID) - 1);
//     selectedSSID[sizeof(selectedSSID) - 1] = '\0';

//     _ui_screen_change(&GUI_Screen__PaswordInputPopUp, LV_SCR_LOAD_ANIM_NONE, 500, 0,
//                       &GUI_initScreen__PaswordInputPopUp);
// }

// static void add_wifi_row(const char *ssid, bool locked) {
//     if (wifi_row_count >= MAX_WIFI_ROWS) return;

//     lv_obj_t *row = lv_button_create(GUI_Container__Available_Networks__container_8);
//     lv_obj_set_size(row, 588, 50);
//     lv_obj_set_style_bg_opa(row, 0, LV_PART_MAIN);
//     lv_obj_set_style_border_side(row, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
//     lv_obj_set_style_border_width(row, 1, LV_PART_MAIN);
//     lv_obj_set_style_border_color(row, lv_color_make(3, 64, 34), LV_PART_MAIN);

//     lv_obj_t *label = lv_label_create(row);
//     lv_label_set_text(label, ssid);
//     lv_obj_set_style_text_color(label, lv_color_make(3, 64, 34), LV_PART_MAIN);
//     lv_obj_align(label, LV_ALIGN_LEFT_MID, 10, 0);

//     if (locked) {
//         lv_obj_t *lock_icon = lv_image_create(row);
//         lv_image_set_src(lock_icon, &upload_frame_38_png);
//         lv_obj_align(lock_icon, LV_ALIGN_RIGHT_MID, -10, 0);
//     }

//     char *ssid_copy = strdup(ssid);
//     lv_obj_add_event_cb(row, wifi_row_clicked_event_cb, LV_EVENT_CLICKED, ssid_copy);

//     wifi_row_objs[wifi_row_count] = row;
//     wifi_row_ssids[wifi_row_count] = ssid_copy;
//     wifi_row_count++;
// }

// static void poll_wifi_scan() {
//     if (!wifi_scan_in_progress) return;

//     int n = WiFi.scanComplete();
//     if (n < 0) return; // still scanning (-1) or not started (-2)

//     clear_wifi_list();
//     for (int i = 0; i < n; i++) {
//         bool locked = WiFi.encryptionType(i) != WIFI_AUTH_OPEN;
//         add_wifi_row(WiFi.SSID(i).c_str(), locked);
//     }
//     WiFi.scanDelete();
//     wifi_scan_in_progress = false;
// }

// static void start_wifi_scan() {
//     if (wifi_scan_in_progress) return;
//     clear_wifi_list();
//     WiFi.scanNetworks(true /* async */);
//     wifi_scan_in_progress = true;
// }

#include "wifiUpdate.h"
#include "GUI.h"
#include <WiFi.h>

#define MAX_WIFI_ROWS 20
static lv_obj_t *wifi_row_objs[MAX_WIFI_ROWS];
static char *wifi_row_ssids[MAX_WIFI_ROWS];
static int wifi_row_count = 0;
static bool wifi_scan_in_progress = false;
static lv_obj_t *last_active_screen = NULL;

char selectedSSID[33] = ""; // the ONE real definition now

static void clear_wifi_list() {
    for (int i = 0; i < wifi_row_count; i++) {
        lv_obj_delete(wifi_row_objs[i]);
        free(wifi_row_ssids[i]);
        wifi_row_objs[i] = NULL;
        wifi_row_ssids[i] = NULL;
    }
    wifi_row_count = 0;
}

static void wifi_row_clicked_event_cb(lv_event_t *e) {
    const char *ssid = (const char *)lv_event_get_user_data(e);
    strncpy(selectedSSID, ssid, sizeof(selectedSSID) - 1);
    selectedSSID[sizeof(selectedSSID) - 1] = '\0';
    _ui_screen_change(&GUI_Screen__PaswordInputPopUp, LV_SCR_LOAD_ANIM_NONE, 0, 0,
                      &GUI_initScreen__PaswordInputPopUp);
}

static void add_wifi_row(const char *ssid, bool locked) {
    if (wifi_row_count >= MAX_WIFI_ROWS) return;

    lv_obj_t *row = lv_button_create(GUI_Container__Available_Networks__container_8);
    lv_obj_set_size(row, 588, 50);
    lv_obj_set_style_bg_opa(row, 0, LV_PART_MAIN);
    lv_obj_set_style_border_side(row, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
    lv_obj_set_style_border_width(row, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(row, lv_color_make(3, 64, 34), LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(row);
    lv_label_set_text(label, ssid);
    lv_obj_set_style_text_color(label, lv_color_make(3, 64, 34), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 10, 0);

    if (locked) {
        lv_obj_t *lock_icon = lv_image_create(row);
        lv_image_set_src(lock_icon, &upload_frame_38_png);
        lv_obj_align(lock_icon, LV_ALIGN_RIGHT_MID, -10, 0);
    }

    char *ssid_copy = strdup(ssid);
    lv_obj_add_event_cb(row, wifi_row_clicked_event_cb, LV_EVENT_CLICKED, ssid_copy);

    wifi_row_objs[wifi_row_count] = row;
    wifi_row_ssids[wifi_row_count] = ssid_copy;
    wifi_row_count++;
}

static void poll_wifi_scan() {
    if (!wifi_scan_in_progress) return;
    int n = WiFi.scanComplete();
    if (n < 0) return;

    clear_wifi_list();
    for (int i = 0; i < n; i++) {
        bool locked = WiFi.encryptionType(i) != WIFI_AUTH_OPEN;
        add_wifi_row(WiFi.SSID(i).c_str(), locked);
    }
    WiFi.scanDelete();
    wifi_scan_in_progress = false;
}

static void start_wifi_scan() {
    if (wifi_scan_in_progress) return;
    clear_wifi_list();
    WiFi.scanNetworks(true);
    wifi_scan_in_progress = true;
}

// void wifi_list_setup() {
//     lv_obj_add_flag(GUI_Button__Available_Networks__AvailableNetworks1, LV_OBJ_FLAG_HIDDEN);
//     lv_obj_set_flex_flow(GUI_Container__Available_Networks__container_8, LV_FLEX_FLOW_COLUMN);
//     lv_obj_add_flag(GUI_Container__Available_Networks__container_8, LV_OBJ_FLAG_SCROLLABLE);
//     lv_obj_set_scroll_dir(GUI_Container__Available_Networks__container_8, LV_DIR_VER);
// }
void wifi_list_setup() {
    lv_obj_add_flag(GUI_Button__Available_Networks__AvailableNetworks1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_flex_flow(GUI_Container__Available_Networks__container_8, LV_FLEX_FLOW_COLUMN);
    lv_obj_add_flag(GUI_Container__Available_Networks__container_8, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scroll_dir(GUI_Container__Available_Networks__container_8, LV_DIR_VER);

    // "WiFi Networks" title was a child of container_8, sharing its flex layout
    // with the dynamic rows -- no amount of repositioning inside that container
    // avoids collision, since the rows flow from the container's own top edge
    // regardless of where the title sits. Moving it out to the screen directly,
    // as a heading positioned just above the white card (container_8's top edge
    // is at y=-174), removes it from that layout entirely.
    lv_obj_set_parent(GUI_Label__Available_Networks__Label_3, GUI_Screen__Available_Networks);
    lv_obj_set_align(GUI_Label__Available_Networks__Label_3, LV_ALIGN_CENTER);
    lv_obj_set_pos(GUI_Label__Available_Networks__Label_3, -224, -130);
    // lv_obj_set_pos(GUI_Label__Available_Networks__Label_3, -250, -200); // was -280, -215
    // Stray duplicate WiFi icon, separate from the real header status icon
    lv_obj_add_flag(GUI_Image__Available_Networks__Image_4, LV_OBJ_FLAG_HIDDEN);
}
void wifi_list_loop() {
    lv_obj_t *current_screen = lv_screen_active();
    if (current_screen != last_active_screen) {
        last_active_screen = current_screen;
        if (current_screen == GUI_Screen__Available_Networks) {
            start_wifi_scan();
        }
    }
    poll_wifi_scan();
}