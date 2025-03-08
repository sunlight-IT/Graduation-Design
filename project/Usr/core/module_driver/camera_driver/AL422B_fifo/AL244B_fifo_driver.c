#include "AL244B_fifo_driver.h"

#include "component/component.h"
#include "core/module_middle/middle_framework/middle_event_process.h"
#define TAG "ALL2448_FIFO"

static uint8_t  n_vsync;
static uint16_t picture_data[CAMERA_WIDTH][CAMERA_HEIGHT];
static bool     pic_state;

void ov7725_fifo_init(void) {
  FIFO_OE_L;
  FIFO_WE_H;
}

void     clear_vsync(void) { n_vsync = 0; }
uint8_t  get_vsync(void) { return n_vsync; }
bool     get_pic_state(void) { return pic_state; }
void     clear_pic_state(void) { pic_state = false; }
uint16_t get_picture_data(uint16_t x, uint16_t y) { return picture_data[x][y]; };
pic_data get_picture(void) { return picture_data; }

void pic_recv(void) {
  uint16_t i, j;
  uint16_t camera_dat = 0;

  FIFO_RD_PREPARE;
  for (i = 0; i < CAMERA_WIDTH; i++) {
    for (j = 0; j < CAMERA_HEIGHT; j++) {
      FIFO_READ(camera_dat);
      picture_data[i][j] = camera_dat;  // 行扫描存放
    }
  }
  clear_vsync();

  pic_state = true;
  // clear_vsync();
}

void vsync_exit(void) {
  if (n_vsync == 0) {
    FIFO_WRST_L;
    FIFO_WE_H;
    n_vsync = 1;
    FIFO_WRST_H;
  } else if (n_vsync == 1) {
    FIFO_WE_L;
    n_vsync = 2;
    enterQueueEvent(EVENT_PicData);
  }
}
