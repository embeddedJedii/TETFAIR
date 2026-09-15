#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMAGE_UPLOAD_RECTANGLE_11_PNG
#define LV_ATTRIBUTE_IMAGE_UPLOAD_RECTANGLE_11_PNG
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMAGE_UPLOAD_RECTANGLE_11_PNG uint8_t upload_rectangle_11_png_map[] = {
  0x22, 0x40, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
  0x00, 
};

const lv_image_dsc_t upload_rectangle_11_png = {
  .header.cf = LV_COLOR_FORMAT_I1,
  .header.magic = LV_IMAGE_HEADER_MAGIC,
  .header.w = 5,
  .header.h = 51,
  .data_size = 59,
  .data = upload_rectangle_11_png_map,
};
