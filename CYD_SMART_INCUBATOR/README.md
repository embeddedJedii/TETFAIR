# SOLHATCH — CYD (ESP32-8048S070C) — ready to build

This is a complete PlatformIO project. Nothing else needs to be downloaded or generated —
open the folder in PlatformIO, plug in the board, build, upload.

## What's included
- `platformio.ini` — board env, partition table, LVGL config path, library dependency
- `boards/esp32-8048S070C.json` — the Sunton board definition (PlatformIO auto-detects the
  project-local `boards/` folder, no submodule step needed)
- `include/lv_conf.h` — LVGL 9.2.2 config, generated from the official template with two
  changes: enabled (`#if 0` → `#if 1`), and `LV_MEM_SIZE` raised from the 64KB default to
  256KB (this UI builds all 10 screens and their widgets up front at boot, which the
  64KB default is too small for)
- `lib/GUI/` — your unzipped SquareLine export, unchanged except for one function in
  `core/gui_core.c`: `GUI_loadFirstScreen()` now loads the splash screen and fades into
  `Home` after exactly 10000 ms, using the exporter's own `_ui_screen_change()` helper
- `src/main.cpp` — calls `smartdisplay_init()` (sets up the display/touch panel and LVGL
  itself) then `GUI_init()` (builds your screens), and drives `lv_timer_handler()` in `loop()`
- `partitions_16mb.csv` — one large ~15MB app partition (no OTA), sized for the ~6-7MB of
  embedded image data your background images compile to

## Build & upload
```
pio run -t upload
pio device monitor
```
(or the equivalent buttons in the PlatformIO IDE extension)

## ⚠️ Before you flash: confirm your board actually has 16MB flash
The `esp32-8048S070C.json` board definition assumes a 16MB-flash, 8MB-PSRAM ESP32-S3 — that's
the standard spec Sunton ships for this model, and it's required here because your six
full-screen backgrounds and two large partial images are embedded as raw, uncompressed
RGB565 arrays (~6-7MB total). If your specific unit shipped with less flash, check with
`esptool.py flash_id` before uploading — the fix in that case is re-exporting those
background images compressed (SquareLine supports this) rather than shrinking the partition
table further.

## If the landing screen after the splash isn't what you want
The splash currently fades into `Home`. To land on a different screen instead, open
`lib/GUI/core/gui_core.c` and swap `GUI_Screen__Home` / `GUI_initScreen__Home` in
`GUI_loadFirstScreen()` for the screen you want (e.g. `GUI_Screen__MotorControl` /
`GUI_initScreen__MotorControl`).
