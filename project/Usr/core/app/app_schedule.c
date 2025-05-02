#include "app_schedule.h"

#include "app_image_display.h"
#include "app_servo.h"

void app_schedule(ui_t* ui) {
  if (!app_trace_picture()) {
    return;
  }
  servo_pid_calculate();
}