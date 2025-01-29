#include "core.h"

#include "component/component.h"
#include "module_apply/lcd/lcd_apply.h"
#include "module_apply/lcd/lcd_camera_image/image_display.h"
#include "module_apply/lcd/pic2.h"
#include "module_apply/voice/ld3320_apply.h"
#include "module_driver/Debug_light_driver/Debug_liget.h"
#include "module_driver/camera_driver/ov7725/ov_7725.h"
#include "module_driver/lcd_driver/lcd_drive.h"
#include "module_driver/servo_driver/servo_driver.h"
#include "module_driver/voice_driver/syn6288_driver/syn6288_driver.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

static ov7725_mode_param_t camera_mode;

static int i;
bool       flag   = false;
int        flages = 1;

void core_init(void) {
  lcd_handle_reg(&hspi2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

  servo_x_handle_reg(&htim2);
  servo_y_handle_reg(&htim4);
  // ld3320_handle_reg(&hspi1);
  syn_handle_uart(&huart2);

  // ld3320_reset();

  voice_compound_cmd("ÄãºÃ");
  // syn6288_inquiry();
  lcd_init();
  Ov7725_init();
  ov7725_mode_config();
  camera_mode = get_camera_mode();
}

void core_loop(void) {
  // switch (flag) {
  //   case true:
  //     flag = false;
  //     image_display();
  //     trace_picture();
  //     break;
  //   default:
  //     if (image_get(camera_mode.lcd_sx,     //
  //                   camera_mode.lcd_sy,     //
  //                   camera_mode.cam_width,  //
  //                   camera_mode.cam_height))
  //       flag = true;
  //     break;
  // }
  // if (flages) {
  //   for (int i = 0; i < 180; i++) {
  //     servo_angle_row(i, 0);
  //     servo_angle_row(i, 1);
  //     HAL_Delay(10);
  //   }
  //   flages = 0;
  // } else {
  //   for (int i = 180; i > 0; i--) {
  //     servo_angle_row(i, 0);
  //     servo_angle_row(i, 1);
  //     HAL_Delay(10);
  //   }
  //   flages = 1;
  // }

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