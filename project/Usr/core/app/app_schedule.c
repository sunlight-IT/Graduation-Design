#include "app_schedule.h"

#include "Core/module_driver/voice_driver/syn6288_driver/syn6288_driver.h"
#include "app_image_display.h"
#include "app_servo.h"
#include "component/component.h"

static bool state_switch[3];

void app_schedule(ui_t* ui) {
  if (get_pic_state()) {
    pic_recv();
    app_image_display();

    if (!app_trace_picture()) {
      return;
    }
    servo_pid_calculate();
  }
}

void app_clore_choose(ui_t* ui) {
  switch (ui->nowItem->id) {
    case COLOR_RED:
      if (state_switch[COLOR_RED]) {
        state_switch[COLOR_RED] = false;
        voice_compound_cmd("ֹͣʶ���ɫ");
      } else {
        state_switch[COLOR_RED] = true;
        voice_compound_cmd("��ʼʶ���ɫ");
      }

      now_target_set(COLOR_RED);
      LOGE("red");
      break;
    case COLOR_YELLOW:

      if (state_switch[COLOR_YELLOW]) {
        state_switch[COLOR_YELLOW] = false;
        voice_compound_cmd("ֹͣʶ���ɫ");
      } else {
        state_switch[COLOR_YELLOW] = true;
        voice_compound_cmd("��ʼʶ���ɫ");
      }
      now_target_set(COLOR_YELLOW);
      break;
    case COLOR_BLUE:
      if (state_switch[COLOR_BLUE]) {
        state_switch[COLOR_BLUE] = false;
        voice_compound_cmd("ֹͣʶ����ɫ");
      } else {
        state_switch[COLOR_BLUE] = true;
        voice_compound_cmd("��ʼʶ����ɫ");
      }
      now_target_set(COLOR_BLUE);
      break;
    default:
      break;
  }
}
