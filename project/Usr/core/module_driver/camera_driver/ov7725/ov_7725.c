#include "ov_7725.h"

#include "../AL422B_fifo/AL244B_fifo_driver.h"
#include "../sccb/sccb.h"
#include "component/component.h"
#include "core/module_driver/lcd_driver/lcd_drive.h"
#define TAG "OV7725_Camera"

typedef struct {
  uint8_t address;
  uint8_t value;
} reg_info_t;

reg_info_t reg_config[] = {
    {k_REG_CLKRC, 0x00}, /*clock config*/
    {k_REG_COM7, 0x46},  /*QVGA RGB565 */
    {k_REG_HSTART, 0x3f},
    {k_REG_HSIZE, 0x50},
    {k_REG_VSTRT, 0x03},
    {k_REG_VSIZE, 0x78},
    {k_REG_HREF, 0x00},
    {k_REG_HOutSize, 0x50},
    {k_REG_VOutSize, 0x78},
    {k_REG_EXHCH, 0x00},

    /*DSP control*/
    {k_REG_TGT_B, 0x7f},
    {k_REG_FixGain, 0x09},
    {k_REG_AWB_Ctrl0, 0xe0},
    {k_REG_DSP_Ctrl1, 0xff},
    {k_REG_DSP_Ctrl2, 0x20},
    {k_REG_DSP_Ctrl3, 0x00},
    {k_REG_DSP_Ctrl4, 0x00},

    /*AGC AEC AWB*/
    {k_REG_COM8, 0xf0},
    {k_REG_COM4, 0x81}, /*Pll AEC CONFIG*/
    {k_REG_COM6, 0xc5},
    {k_REG_COM9, 0x21},
    {k_REG_BDBase, 0xFF},
    {k_REG_BDMStep, 0x01},
    {k_REG_AEW, 0x34},
    {k_REG_AEB, 0x3c},
    {k_REG_VPT, 0xa1},
    {k_REG_EXHCL, 0x00},
    {k_REG_AWBCtrl3, 0xaa},
    {k_REG_COM8, 0xff},
    {k_REG_AWBCtrl1, 0x5d},

    {k_REG_EDGE1, 0x0a},
    {k_REG_DNSOff, 0x01},
    {k_REG_EDGE2, 0x01},
    {k_REG_EDGE3, 0x01},

    {k_REG_MTX1, 0x5f},
    {k_REG_MTX2, 0x53},
    {k_REG_MTX3, 0x11},
    {k_REG_MTX4, 0x1a},
    {k_REG_MTX5, 0x3d},
    {k_REG_MTX6, 0x5a},
    {k_REG_MTX_Ctrl, 0x1e},

    {k_REG_BRIGHT, 0x00},
    {k_REG_CNST, 0x25},
    {k_REG_USAT, 0x65},
    {k_REG_VSAT, 0x65},
    {k_REG_UVADJ0, 0x81},
    //{k_REG_SDE,		  0x20},	//黑白
    {k_REG_SDE, 0x06},  // 彩色	调节SDE这个寄存器还可以实现其他效果

    /*GAMMA config*/
    {k_REG_GAM1, 0x0c},
    {k_REG_GAM2, 0x16},
    {k_REG_GAM3, 0x2a},
    {k_REG_GAM4, 0x4e},
    {k_REG_GAM5, 0x61},
    {k_REG_GAM6, 0x6f},
    {k_REG_GAM7, 0x7b},
    {k_REG_GAM8, 0x86},
    {k_REG_GAM9, 0x8e},
    {k_REG_GAM10, 0x97},
    {k_REG_GAM11, 0xa4},
    {k_REG_GAM12, 0xaf},
    {k_REG_GAM13, 0xc5},
    {k_REG_GAM14, 0xd7},
    {k_REG_GAM15, 0xe8},
    {k_REG_SLOP, 0x20},

    {k_REG_HUECOS, 0x80},
    {k_REG_HUESIN, 0x80},
    {k_REG_DSPAuto, 0xff},
    {k_REG_DM_LNL, 0x00},
    {k_REG_BDBase, 0x99},
    {k_REG_BDMStep, 0x03},
    {k_REG_LC_RADI, 0x00},
    {k_REG_LC_COEF, 0x13},
    {k_REG_LC_XC, 0x08},
    {k_REG_LC_COEFB, 0x14},
    {k_REG_LC_COEFR, 0x17},
    {k_REG_LC_CTR, 0x05},

    {k_REG_COM3, 0xd0}, /*Horizontal mirror image*/

    /*night mode auto frame rate control*/
    {k_REG_COM5, 0xf5}, /*在夜视环境下，自动降低帧率，保证低照度画面质量*/
                        //{REG_COM5,		0x31},	/*夜视环境帧率不变*/
};
static uint8_t reg_num = sizeof(reg_config) / sizeof(reg_config[0]);

static ov7725_mode_param_t camera_mode = {
    .QVGA_VGA = k_QVGA,
    .cam_sx   = 0,
    .cam_sy   = 0,

    .cam_width  = 120,
    .cam_height = 120,

    .lcd_sx   = 0,
    .lcd_sy   = 0,
    .lcd_scan = 3,

    .light_mode = k_office,  // 自动光照模式
    .saturation = 4,
    .brightness = 0,
    .contrast   = 2,
    .effect     = k_normal,  // 反相
};

bool Ov7725_init(void) {
  uint8_t ID = 0;
  ov7725_fifo_init();

  if (0 == sccb_write_byte(0x12, 0x80)) {
    ErrorHanding(TAG, "sccb_write_byte error");
    return false;
  }
  if (0 == sccb_read_byte(0x0b, &ID, 1)) {
    ErrorHanding(TAG, "sccb_read_byte error");
    return false;
  }

  if (ID != OV7725_ID) {
    ErrorHanding(TAG, "ID error");
    return false;
  }
  ZLOGI(TAG, "%02x", ID);

  for (int i = 0; i < reg_num; i++) {
    if (0 == sccb_write_byte(reg_config[i].address, reg_config[i].value)) {
      ZLOGE(TAG, "sccb_write_byte %02x error", reg_config[i].address);
      return false;
    }
  }
  ZLOGI(TAG, "ov7725 Rgeister Config Sucess");
  return true;
}

// Light Mode
void ov7725_light_mode(light_mode mode) {
  switch (mode) {
    case k_auto:                    // Auto，自动模式
      sccb_write_byte(0x13, 0xff);  // AWB on
      sccb_write_byte(0x0e, 0x65);
      sccb_write_byte(0x2d, 0x00);
      sccb_write_byte(0x2e, 0x00);
      break;
    case k_sunny:                   // sunny，晴天
      sccb_write_byte(0x13, 0xfd);  // AWB off
      sccb_write_byte(0x01, 0x5a);
      sccb_write_byte(0x02, 0x5c);
      sccb_write_byte(0x0e, 0x65);
      sccb_write_byte(0x2d, 0x00);
      sccb_write_byte(0x2e, 0x00);
      break;
    case k_cloudy:                  // cloudy，多云
      sccb_write_byte(0x13, 0xfd);  // AWB off
      sccb_write_byte(0x01, 0x58);
      sccb_write_byte(0x02, 0x60);
      sccb_write_byte(0x0e, 0x65);
      sccb_write_byte(0x2d, 0x00);
      sccb_write_byte(0x2e, 0x00);
      break;
    case k_office:                  // office，办公室
      sccb_write_byte(0x13, 0xfd);  // AWB off
      sccb_write_byte(0x01, 0x84);
      sccb_write_byte(0x02, 0x4c);
      sccb_write_byte(0x0e, 0x65);
      sccb_write_byte(0x2d, 0x00);
      sccb_write_byte(0x2e, 0x00);
      break;
    case k_home:                    // home，家里
      sccb_write_byte(0x13, 0xfd);  // AWB off
      sccb_write_byte(0x01, 0x96);
      sccb_write_byte(0x02, 0x40);
      sccb_write_byte(0x0e, 0x65);
      sccb_write_byte(0x2d, 0x00);
      sccb_write_byte(0x2e, 0x00);
      break;

    case k_night:                   // night，夜晚
      sccb_write_byte(0x13, 0xff);  // AWB on
      sccb_write_byte(0x0e, 0xe5);
      break;

    default:
      ZLOGE(TAG, "Light Mode parameter error! : %d", mode);

      break;
  }
}

// Color Saturation  饱和度
void ov7725_color_saturation(int8_t sat) {
  if (sat >= -4 && sat <= 4) {
    sccb_write_byte(k_REG_USAT, (sat + 4) << 4);
    sccb_write_byte(k_REG_VSAT, (sat + 4) << 4);
  } else {
    ZLOGE(TAG, "Color Saturation parameter error! :%d", sat);
  }
}

// Brightness 光照强度
void ov7725_brightness(int8_t brightness) {
  uint8_t BRIGHT_Value, SIGN_Value;

  switch (brightness) {
    case 4:
      BRIGHT_Value = 0x48;
      SIGN_Value   = 0x06;
      break;

    case 3:
      BRIGHT_Value = 0x38;
      SIGN_Value   = 0x06;
      break;

    case 2:
      BRIGHT_Value = 0x28;
      SIGN_Value   = 0x06;
      break;

    case 1:
      BRIGHT_Value = 0x18;
      SIGN_Value   = 0x06;
      break;

    case 0:
      BRIGHT_Value = 0x08;
      SIGN_Value   = 0x06;
      break;

    case -1:
      BRIGHT_Value = 0x08;
      SIGN_Value   = 0x0e;
      break;

    case -2:
      BRIGHT_Value = 0x18;
      SIGN_Value   = 0x0e;
      break;

    case -3:
      BRIGHT_Value = 0x28;
      SIGN_Value   = 0x0e;
      break;

    case -4:
      BRIGHT_Value = 0x38;
      SIGN_Value   = 0x0e;
      break;

    default:
      ZLOGE(TAG, "Brightness parameter error! : %d", brightness);
      break;
  }

  sccb_write_byte(k_REG_BRIGHT, BRIGHT_Value);  // AWB on
  sccb_write_byte(k_REG_SIGN, SIGN_Value);
}

// Contrast 对比度
void ov7725_constrast(int8_t cnst) {
  if (cnst >= -4 && cnst <= 4) {
    sccb_write_byte(k_REG_CNST, (0x30 - (4 - cnst) * 4));
  } else {
    ZLOGE(TAG, "Contrast parameter error! : %d", cnst);
  }
}

// Special effects 特殊效果

void ov7725_special_effect(special_effects eff) {
  switch (eff) {
    case k_normal:  // 正常
      sccb_write_byte(0xa6, 0x06);
      sccb_write_byte(0x60, 0x80);
      sccb_write_byte(0x61, 0x80);
      break;

    case k_balck_white:  // 黑白
      sccb_write_byte(0xa6, 0x26);
      sccb_write_byte(0x60, 0x80);
      sccb_write_byte(0x61, 0x80);
      break;

    case k_bluish:  // 偏蓝
      sccb_write_byte(0xa6, 0x1e);
      sccb_write_byte(0x60, 0xa0);
      sccb_write_byte(0x61, 0x40);
      break;

    case k_sepia:  // 复古
      sccb_write_byte(0xa6, 0x1e);
      sccb_write_byte(0x60, 0x40);
      sccb_write_byte(0x61, 0xa0);
      break;

    case k_redish:  // 偏红
      sccb_write_byte(0xa6, 0x1e);
      sccb_write_byte(0x60, 0x80);
      sccb_write_byte(0x61, 0xc0);
      break;

    case k_greenish:  // 偏绿
      sccb_write_byte(0xa6, 0x1e);
      sccb_write_byte(0x60, 0x60);
      sccb_write_byte(0x61, 0x60);
      break;

    case k_negative:  // 反相
      sccb_write_byte(0xa6, 0x46);
      break;

    default:
      ZLOGE(TAG, "Special Effect error! : %d", eff);
      break;
  }
}

void ov7725_window_set(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height, resolution_param QVGA_VGA) {
  uint8_t reg_raw = 0, cal_temp = 0;

  if (k_QVGA == QVGA_VGA)
    sccb_write_byte(k_REG_COM7, 0x46);
  else if (k_VGA == QVGA_VGA)
    sccb_write_byte(k_REG_COM7, 0x06);

  /***************HSTART*********************/
  // 读取寄存器的原内容，HStart包含偏移值，在原始偏移植的基础上加上窗口偏移
  sccb_read_byte(k_REG_HSTART, &reg_raw, 1);

  // sx为窗口偏移，高8位存储在HSTART，低2位在HREF
  cal_temp = (reg_raw + (sx >> 2));
  sccb_write_byte(k_REG_HSTART, cal_temp);

  /***************HSIZE*********************/
  // 水平宽度，高8位存储在HSIZE，低2位存储在HREF
  sccb_write_byte(k_REG_HSIZE, width >> 2);  // HSIZE左移两位

  /***************VSTART*********************/
  // 读取寄存器的原内容，VStart包含偏移值，在原始偏移植的基础上加上窗口偏移
  sccb_read_byte(k_REG_VSTRT, &reg_raw, 1);
  // sy为窗口偏移，高8位存储在HSTART，低1位在HREF
  cal_temp = (reg_raw + (sy >> 1));

  sccb_write_byte(k_REG_VSTRT, cal_temp);

  /***************VSIZE*********************/
  // 垂直高度，高8位存储在VSIZE，低1位存储在HREF
  sccb_write_byte(k_REG_VSIZE, height >> 1);  // VSIZE左移一位

  /***************VSTART*********************/
  // 读取寄存器的原内容
  sccb_read_byte(k_REG_HREF, &reg_raw, 1);
  // 把水平宽度的低2位、垂直高度的低1位，水平偏移的低2位，垂直偏移的低1位的配置添加到HREF
  cal_temp = (reg_raw | (width & 0x03) | ((height & 0x01) << 2) | ((sx & 0x03) << 4) | ((sy & 0x01) << 6));

  sccb_write_byte(k_REG_HREF, cal_temp);

  /***************HOUTSIZIE /VOUTSIZE*********************/
  sccb_write_byte(k_REG_HOutSize, width >> 2);
  sccb_write_byte(k_REG_VOutSize, height >> 1);

  // 读取寄存器的原内容
  sccb_read_byte(k_REG_EXHCH, &reg_raw, 1);
  cal_temp = (reg_raw | (width & 0x03) | ((height & 0x01) << 2));

  sccb_write_byte(k_REG_EXHCH, cal_temp);
}
ov7725_mode_param_t get_camera_mode(void) { return camera_mode; }

void ov7725_mode_config(void) {
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
  // lcd_set_windows(0, 0, 160 - 1, 120 - 1);
  clear_vsync();
}