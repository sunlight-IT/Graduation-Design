#include "app_schedule.h"

#include "app_image_display.h"
#include "app_servo.h"
#include "Core/module_driver/voice_driver/syn6288_driver/syn6288_driver.h"

void app_schedule(ui_t* ui) {
  if (!app_trace_picture()) {
    return;
  }
  servo_pid_calculate();
}


void app_clore_choose(ui_t* ui) {
switch(ui->nowItem->id){
  case COLOR_RED: 
  voice_compound_cmd("开始识别红色");
  now_target_set(COLOR_RED);
    break;
  case COLOR_YELLOW:
  voice_compound_cmd("开始识别黄色");
    now_target_set(COLOR_YELLOW);
    break;
  case COLOR_BLUE:
  voice_compound_cmd("开始识别蓝色");
    now_target_set(COLOR_BLUE);
    break;
  default:
    break;
}
}
