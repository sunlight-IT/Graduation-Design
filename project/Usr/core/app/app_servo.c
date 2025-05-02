#include "app_servo.h"

#include "Core/module_middle/image_display/middle_image.h"
#include "component/algorithm/pid/pid.h"
#define PI 3.1415926

static POSITION error_coord;

void rate_translate_angle(void) {
  error_coordinate_cal();
  double ret;
  float  tanx = (float)(error_coord.x * CAL_PARAMER);
  float  tany = (float)(error_coord.y * CAL_PARAMER);
  ret         = atanf(tanx) * 180 / PI;

  lcd_show_string(0, 210, 12, "error is: ", 0);
  lcd_show_string(0, 225, 12, "(          )", 0);
  lcd_show_num(12, 225, error_coord.x, 2, 12);
  lcd_show_num(35, 225, error_coord.y, 2, 12);
}

void servo_pid_calculate(void) {
  int32_t  out_x = 0;
  int32_t  out_y = 0;
  POSITION pos   = get_trace_pos();
  error_coord.x  = (uint16_t)RGB_PIC_CENTER_X - pos.x;
  error_coord.y  = (uint16_t)RGB_PIC_CENTER_Y - pos.y;
  out_x          = pid_increment(error_coord.x, k_pid_servo_x);
  out_y          = pid_increment(error_coord.y, k_pid_servo_y);
  servo_angle_row(MID_ANGLR + out_x, 0);
  servo_angle_row(MID_ANGLR + out_y, 1);
  // lcd_show_string(0, 210, 12, "error is: ", 0);
  // lcd_show_string(0, 225, 12, "(          )", 0);
  // lcd_show_num(12, 225, error_coord.x, 2, 12);
  // lcd_show_num(35, 225, error_coord.y, 2, 12);

  // lcd_show_string(200, 200, 12, "out is ", 0);
  // lcd_show_num(200, 215, out_x, 2, 12);
  // lcd_show_num(200, 225, out_y, 2, 12);
}

POSITION* get_error_coord(void) { return &error_coord; }