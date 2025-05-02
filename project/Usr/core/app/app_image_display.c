#include "app_image_display.h"

#include "Core/module_middle/image_display/middle_image.h"
#include "component/algorithm/easy_trace/EasyTracer_color.h"

static TARGET_CONDI target_conditon = {0, 20, 210, 255, 100, 255, 60, 200, 10, 10, 100, 100};
// static uint16_t     picture_data[PIC_W][PIC_H];
// static uint8_t      picture_rgb_binarization[PIC_W][PIC_H];
static RESULT   result;
static POSITION result_pos;

bool app_trace_picture(void) {
  // ZLOGE(TAG, "trace ");
  if (!Trace(&target_conditon, &result)) {
    return false;
  }

  result_pos.x = result.x;
  result_pos.y = result.y;
  return true;
}

void app_image_display(void) {
  // image_display_rgb();
  // lcd_draw_cross(result.x, result.y, 3);
  // lcd_draw_rectangle(result.x, result.y, result.w, result.h);

  // image_display_rgb(pic);
  image_display_bin();

  lcd_show_string(0, 150, 12, "trace center is ", 0);
  lcd_show_string(0, 165, 12, "(  ,  )", 0);
  lcd_show_num(5, 165, result.x, 2, 12);
  lcd_show_num(22, 165, result.y, 2, 12);

  lcd_show_string(0, 180, 12, "picture center is ", 0);
  lcd_show_string(0, 195, 12, "(  ,  )", 0);
  lcd_show_num(5, 195, RGB_PIC_CENTER_X, 2, 12);
  lcd_show_num(22, 195, RGB_PIC_CENTER_Y, 2, 12);
}