#include "image_display.h"

#include <stdlib.h>

#include "component/algorithm/easy_trace/EasyTracer_color.h"
#include "component/component.h"
#include "core/module_driver/camera_driver/AL422B_fifo/AL244B_fifo_driver.h"
#include "core/module_driver/lcd_driver/lcd_drive.h"

#define TAG "Image_display"

#define min3v(v1, v2, v3) ((v1) > (v2) ? ((v2) > (v3) ? (v3) : (v2)) : ((v1) > (v3) ? (v3) : (v1)))  // 取最大值
#define max3v(v1, v2, v3) ((v1) < (v2) ? ((v2) < (v3) ? (v3) : (v2)) : ((v1) < (v3) ? (v3) : (v1)))  // 取最小值

static TARGET_CONDI target_conditon = {0, 20, 210, 255, 100, 255, 60, 200, 10, 10, 100, 100};
static uint16_t     picture_data[PIC_W][PIC_H];
// static uint8_t      picture_rgb_binarization[PIC_W][PIC_H];
static RESULT   result;
static POSITION result_pos;

static void    read_color(unsigned int x, unsigned int y, rgb_t* Rgb);
static void    rgb_to_hsl(const rgb_t* Rgb, hsl_t* Hsl);
static uint8_t color_match_bin(hsl_t* hsl);

bool image_get(uint8_t sx, uint8_t sy, uint16_t width, uint16_t height) {
  bool     status = false;
  uint16_t i, j;
  uint16_t camera_dat = 0;

  if (get_vsync() == 2) {
    FIFO_RD_PREPARE;
    for (i = 0; i < width; i++) {
      for (j = 0; j < height; j++) {
        FIFO_READ(camera_dat);
        picture_data[i][j] = camera_dat;  // 行扫描存放
      }
    }
    clear_vsync();
    return true;
  }
  return false;
}

void image_display_bin(void) {
  int      i, j;
  rgb_t    rgb_tmp;
  hsl_t    hsl_tmp;
  uint16_t tmp_data = 0;
  lcd_set_windows(BIN_SHOW_X, 0, BIN_SHOW_X + PIC_W - 1, 0 + PIC_H - 1);
  for (i = 0; i < PIC_W; i++) {
    for (j = 0; j < PIC_H; j++) {
      read_color(i, j, &rgb_tmp);  // 行扫描读取
      rgb_to_hsl(&rgb_tmp, &hsl_tmp);
      // picture_rgb_binarization[i][j] = color_match_bin(&hsl_tmp);
      // lcd_write_data_16(picture_rgb_binarization[j][i]);  // 屏幕的扫描方式是列扫描，所以得转置
      lcd_write_data_16(color_match_bin(&hsl_tmp));
    }
  }
}
void image_display_rgb(void) {
  int      i, j;
  uint16_t tmp_data = 0;
  lcd_set_windows(RGB_SHOW_X, RGB_SHOW_Y, RGB_SHOW_X + PIC_W - 1, RGB_SHOW_Y + PIC_H - 1);
  for (i = 0; i < PIC_W; i++)
    for (j = 0; j < PIC_H; j++) {
      tmp_data = picture_data[j][i] >> 8;
      tmp_data |= picture_data[j][i] << 8;
      lcd_write_data_16(tmp_data);
    }
}
void image_display(void) {
  // image_display_rgb();
  // lcd_draw_cross(result.x, result.y, 3);
  // lcd_draw_rectangle(result.x, result.y, result.w, result.h);

  image_display_bin();

  lcd_show_string(0, 150, 12, "trace center is ", 0);
  lcd_show_string(0, 165, 12, "(  ,  )", 0);
  lcd_show_num(5, 165, result.x, 2, 12);
  lcd_show_num(22, 165, result.y, 2, 12);

  lcd_show_string(0, 180, 12, "picture center is ", 0);
  lcd_show_string(0, 195, 12, "(  ,  )", 0);
  lcd_show_num(5, 195, RGB_PIC_CENTER_X, 2, 12);
  lcd_show_num(22, 195, RGB_PIC_CENTER_Y, 2, 12);
}

bool finding_flag = false;
bool trace_picture(void) {
  if (Trace(&target_conditon, &result)) {
    result_pos.x = result.x;
    result_pos.y = result.y;
    // ZLOGI(TAG, "trace sucess");
    return true;
  } else {
    return false;
    // result_pos.x = 0;
    // result_pos.y = 0;
  }
  // ZLOGE(TAG, "trace fail");
}

uint16_t get_picture_data(uint16_t x, uint16_t y) { return picture_data[x][y]; }
// uint16_t get_picture_data_bina(uint16_t x, uint16_t y) { return picture_rgb_binarization[x][y]; }
POSITION get_trace_pos() { return result_pos; }

static void rgb_to_hsl(const rgb_t* Rgb, hsl_t* Hsl) {
  int h, s, l, maxVal, minVal, difVal;
  int r = Rgb->r;
  int g = Rgb->g;
  int b = Rgb->b;  // 提取R、G、B值

  maxVal = max3v(r, g, b);
  minVal = min3v(r, g, b);   // 提取RGB三者最大/最小值
  difVal = maxVal - minVal;  // 计算最大最小值的差值

  // 计算亮度
  l = (maxVal + minVal) * 240 / 255 / 2;
  if (maxVal == minVal)  // 若r=g=b
  {
    h = 0;
    s = 0;
  } else {
    // 计算色调
    if (maxVal == r) {
      if (g >= b)
        h = 40 * (g - b) / (difVal);
      else
        h = 40 * (g - b) / (difVal) + 240;
    } else if (maxVal == g)
      h = 40 * (b - r) / (difVal) + 80;
    else if (maxVal == b)
      h = 40 * (r - g) / (difVal) + 160;
    // 计算饱和度
    if (l == 0)
      //	s = 0;
      s = (difVal) * 240 / (511 - (maxVal + minVal));
    else if (l <= 120)
      s = (difVal) * 240 / (maxVal + minVal);
    else
      s = (difVal) * 240 / (511 - (maxVal + minVal));
  }
  Hsl->h = (unsigned char)(((h > 240) ? 240 : ((h < 0) ? 0 : h)));  // 色度
  Hsl->s = (unsigned char)(((s > 240) ? 240 : ((s < 0) ? 0 : s)));  // 饱和度
  Hsl->l = (unsigned char)(((l > 240) ? 240 : ((l < 0) ? 0 : l)));  // 亮度
}
static void read_color(unsigned int x, unsigned int y, rgb_t* Rgb) {
  uint16_t C16;  // 即Color16，R5+G6+B5=16

  C16    = get_picture_data(y, x);  // 读某点颜色
  Rgb->r = (unsigned char)((C16 & 0xf800) >> 8);
  Rgb->g = (unsigned char)((C16 & 0x07e0) >> 3);
  Rgb->b = (unsigned char)((C16 & 0x001f) << 3);
}

static uint8_t color_match_bin(hsl_t* hsl) {
  if (((hsl->h > target_conditon.H_MIN1 && hsl->h < target_conditon.H_MAX1) || (hsl->h > target_conditon.H_MIN2 && hsl->h < target_conditon.H_MAX2))  //
      && hsl->s > target_conditon.S_MIN && hsl->s < target_conditon.S_MAX                                                                             //
      && hsl->l > target_conditon.L_MIN && hsl->l < target_conditon.L_MAX) {
    return 0;
  } else
    return 255;
}