#include "core.h"

#include "app/app_image_display.h"
#include "app/app_schedule.h"
#include "component/component.h"
#include "component/time_slice/time_slice.h"
// #include "fsm_middle/fsm_middle.h"
#include "module_driver/Debug_light_driver/Debug_liget.h"
#include "module_driver/camera_driver/AL422B_fifo/AL244B_fifo_driver.h"
#include "module_driver/camera_driver/ov7725/ov_7725.h"
#include "module_driver/lcd_driver/lcd_drive.h"
#include "module_driver/servo_driver/servo_driver.h"
#include "module_driver/voice_driver/syn6288_driver/syn6288_driver.h"
#include "module_middle/middle_framework/middle_event_process.h"
#include "module_middle/middle_framework/middle_table.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
//
#include "ui/MiaoUI/core_ui/ui.h"
#include "ui/MiaoUI/ui_conf.h"
//

//
#include "app/app_button.h"
//

#define DEBUG_OLED 1

static ov7725_mode_param_t camera_mode;

static int  i;
bool        flag = false;
extern bool finding_flag;

static ui_t ui;
void        core_init(void) {
#if DEBUG_OLED
  HAL_TIM_Base_Start_IT(&htim3);

  lcd_handle_reg(&hspi2);

  servo_x_handle_reg(&htim4);
  servo_y_handle_reg(&htim2);
  /******************************* */
  lcd_init();

  Ov7725_init();
  ov7725_mode_config();
  camera_mode = get_camera_mode();

  servo_angle_row(90, 0);
  servo_angle_row(90, 1);

  display_init();
  // registCallback(EVENT_PicData, pic_recv);
#endif
  // ld3320_handle_reg(&hspi1);
  // ld3320_reset();

  syn_handle_uart(&huart2);
   voice_compound_cmd("语音初始化完成");

  // syn6288_inquiry();

   

   APP_Button_SetUp();
   MiaoUi_Setup(&ui);
}

void core_loop(void) {
  if (true == get_pic_state()) {
    app_image_display();
  }

  ui_loop(&ui);

  // process_event();

  servo_test();
  debug_light();
}

void core_main(void) {
  // setup
  core_init();

  while (1) {
    // loop
    core_loop();
  }
}