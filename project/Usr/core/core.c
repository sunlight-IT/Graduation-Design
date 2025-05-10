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

static uint8_t test1[] = {0xFD, 0x00, 0x02, 0x21, 0xDE};
void           core_init(void) {
#if DEBUG_OLED
  HAL_TIM_Base_Start_IT(&htim3);

  // lcd_handle_reg(&hspi2);
  syn_handle_uart(&huart3);

  servo_x_handle_reg(&htim2);
  servo_y_handle_reg(&htim4);
  /******************************* */
  lcd_init();
  Ov7725_init();
  ov7725_mode_config();
  camera_mode = get_camera_mode();

#endif
  HAL_Delay(100);
  voice_inquire_cmd();
  HAL_Delay(10);
  voice_compound_cmd("初始化成功");
  HAL_Delay(2000);

  display_init();
  APP_Button_SetUp();
  MiaoUi_Setup(&ui);

  servo_angle_row(1350, 0);
  servo_angle_row(1350, 1);
  HAL_Delay(1000);
}
void core_loop(void) {
  ui_loop(&ui);
  debug_light();
}

void core_main(void) {
  //   setup路路
  core_init();

  while (1) {
    // loop
    core_loop();
  }
}
