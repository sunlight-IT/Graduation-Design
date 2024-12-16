#include "core.h"

#include "component/component.h"
#include "module_apply/lcd/lcd_apply.h"
#include "module_apply/lcd/lcd_camera_image/image_display.h"
#include "module_apply/lcd/pic2.h"
#include "module_driver/Debug_light_driver/Debug_liget.h"
#include "module_driver/camera_driver/ov7725/ov_7725.h"
#include "module_driver/lcd_driver/lcd_drive.h"
#include "module_driver/servo_driver/servo_driver.h"
#include "spi.h"
#include "tim.h"

static ov7725_mode_param_t camera_mode;

static int i;
bool       flag = true;

void core_init(void) {
  lcd_handle_reg(&hspi2);
  lcd_init();
  // lcd_draw_line(3, 100, 3, 100);
  // HAL_Delay(100);
  // gui_draw_image(0, 0, 180 - 1, 180 - 1, gImage_pic2);
  Ov7725_init();
  ov7725_mode_config();
  // test();

  camera_mode = get_camera_mode();
  // HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  // servo_handle_reg(&htim2);
}

void core_loop(void) {
  // if (flag)
  //   ++i;
  // else
  //   --i;
  // image_get(camera_mode.lcd_sx,     //
  //           camera_mode.lcd_sy,     //
  //           camera_mode.cam_width,  //
  //           camera_mode.cam_height);
  // servo_angle_row(i);
  // if (i >= 180)
  //   flag = false;
  // else if (i <= 0)
  //   flag = true;
  switch (flag) {
    case 1:
      flag = 0;
      image_display();
      trace_picture();

      break;
    default:

      if (image_get(camera_mode.lcd_sx,     //
                    camera_mode.lcd_sy,     //
                    camera_mode.cam_width,  //
                    camera_mode.cam_height))
        flag = 1;
      break;
  }

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