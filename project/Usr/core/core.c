#include "core.h"

#include "component/component.h"
#include "component/time_slice/time_slice.h"
#include "fsm_middle/fsm_middle.h"
#include "module_driver/Debug_light_driver/Debug_liget.h"
#include "module_driver/camera_driver/AL422B_fifo/AL244B_fifo_driver.h"
#include "module_driver/lcd_driver/lcd_drive.h"
#include "module_driver/servo_driver/servo_driver.h"
#include "module_driver/voice_driver/syn6288_driver/syn6288_driver.h"
#include "module_middle/image_display/image_display.h"
#include "module_middle/middle_framework/middle_event_process.h"
#include "module_middle/middle_framework/middle_table.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

static ov7725_mode_param_t camera_mode;

static int  i;
bool        flag = false;
extern bool finding_flag;
void        core_init(void) {
  lcd_handle_reg(&hspi2);

  servo_x_handle_reg(&htim4);
  servo_y_handle_reg(&htim2);
  /******************************* */
  lcd_init();

  servo_angle_row(900, 0);
  servo_angle_row(900, 1);
  // ld3320_handle_reg(&hspi1);
  // ld3320_reset();

  // syn_handle_uart(&huart2);
  // voice_compound_cmd("ÄãºÃ");

  // syn6288_inquiry();

  Ov7725_init();
  ov7725_mode_config();
  camera_mode = get_camera_mode();

  registCallback(EVENT_PicData, pic_recv);
}

extern TIME_DATA_T image_data;
extern TIME_DATA_T image_show;
extern TIME_DATA_T servo_ctrl;
void               core_loop(void) {
  // servo_test();

  // if (image_process(&camera_mode)) {
  //   image_display(get_picture());
  //   servo_pid_calculate();
  // }

  if (true == get_pic_state()) {
    image_display(get_picture());
  }
  process_event();
  // process_table();

  // if (image_data.state) {
  //   if (image_get(camera_mode.lcd_sx,     //
  //                               camera_mode.lcd_sy,     //
  //                               camera_mode.cam_width,  //
  //                               camera_mode.cam_height))
  //     trace_picture();
  //   image_data.state = false;
  // }
  // if (image_show.state) {
  //   image_show.state = false;
  // }
  // if (servo_ctrl.state && finding_flag) {
  //   servo_ctrl.state = false;
  // }

  //

  //  voice_apply();
  //  syn_recv();
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