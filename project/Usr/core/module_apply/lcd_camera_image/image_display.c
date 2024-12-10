#include "image_display.h"

#include "component/component.h"
#include "core/module_driver/camera_driver/AL422B_fifo/AL244B_fifo_driver.h"
#include "core/module_driver/lcd_driver/lcd_drive.h"

#define TAG "Image_display"

void image_display(uint8_t sx, uint8_t sy, uint16_t width, uint16_t height) {
  uint16_t i, j;
  uint16_t camera_data = 0;
  if (get_vsync() == 2) {
    __disable_irq();
    // ZLOGI(TAG, "image output");
    FIFO_RD_PREPARE;
    lcd_set_windows(sx, sy, sx + width - 1, sy + height - 1);
    for (i = 0; i < width; i++) {
      for (j = 0; j < height; j++) {
        // 一个像素数据为R565类型，一个R565类型数据大小为2个byte
        FIFO_READ(camera_data);
        // ZLOGI(TAG, "camera_data is %02x", camera_data);
        lcd_write_data_16(camera_data);
      }
    }
    // ZLOGI(TAG, "camera_data is %02x", camera_data);
    clear_vsync();
    __enable_irq();
  }
}