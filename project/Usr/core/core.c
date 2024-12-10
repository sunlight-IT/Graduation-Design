#include "core.h"

#include "component/component.h"
#include "module_apply/GUI/gui.h"
#include "module_apply/GUI/pic2.h"
#include "module_apply/lcd_camera_image/image_display.h"
#include "module_driver/Debug_light_driver/Debug_liget.h"
#include "module_driver/camera_driver/AL422B_fifo/AL244B_fifo_driver.h"
#include "module_driver/camera_driver/ov7725/ov_7725.h"
#include "module_driver/lcd_driver/lcd_drive.h"
#include "spi.h"

static ov7725_mode_param_t camera_mode;

void core_init(void) {
  camera_mode = get_camera_mode();
  lcd_handle_reg(&hspi2);
  lcd_init();
  // lcd_draw_line(3, 100, 3, 100);
  // HAL_Delay(100);
  // gui_draw_image(0, 0, 180 - 1, 180 - 1, gImage_pic2);
  ov7725_fifo_init();
  Ov7725_init();

  ov7725_light_mode(camera_mode.light_mode);
  ov7725_color_saturation(camera_mode.saturation);
  ov7725_brightness(camera_mode.brightness);
  ov7725_constrast(camera_mode.contrast);
  ov7725_special_effect(camera_mode.effect);
  ov7725_window_set(camera_mode.cam_sx,      //
                    camera_mode.cam_sy,      //
                    camera_mode.cam_width,   //
                    camera_mode.cam_height,  //
                    camera_mode.QVGA_VGA);
  lcd_direction(camera_mode.lcd_scan);
  // lcd_set_windows(0, 0, 240 - 1, 320 - 1);
  clear_vsync();
}

void core_loop(void) {
  image_display(camera_mode.lcd_sx,     //
                camera_mode.lcd_sy,     //
                camera_mode.cam_width,  //
                camera_mode.cam_height);
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