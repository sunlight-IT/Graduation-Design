#pragma once

#include <stdbool.h>
#include <stdint.h>
#define OV7725_ID 0x21

typedef enum {
  k_REG_GAIN      = 0x00,
  k_REG_BLUE      = 0x01,
  k_REG_RED       = 0x02,
  k_REG_GREEN     = 0x03,
  k_REG_BAVG      = 0x05,
  k_REG_GAVG      = 0x06,
  k_REG_RAVG      = 0x07,
  k_REG_AECH      = 0x08,
  k_REG_COM2      = 0x09,
  k_REG_PID       = 0x0A,
  k_REG_VER       = 0x0B,
  k_REG_COM3      = 0x0C,
  k_REG_COM4      = 0x0D,
  k_REG_COM5      = 0x0E,
  k_REG_COM6      = 0x0F,
  k_REG_AEC       = 0x10,
  k_REG_CLKRC     = 0x11,
  k_REG_COM7      = 0x12,
  k_REG_COM8      = 0x13,
  k_REG_COM9      = 0x14,
  k_REG_COM10     = 0x15,
  k_REG_REG16     = 0x16,
  k_REG_HSTART    = 0x17,
  k_REG_HSIZE     = 0x18,
  k_REG_VSTRT     = 0x19,
  k_REG_VSIZE     = 0x1A,
  k_REG_PSHFT     = 0x1B,
  k_REG_MIDH      = 0x1C,
  k_REG_MIDL      = 0x1D,
  k_REG_LAEC      = 0x1F,
  k_REG_COM11     = 0x20,
  k_REG_BDBase    = 0x22,
  k_REG_BDMStep   = 0x23,
  k_REG_AEW       = 0x24,
  k_REG_AEB       = 0x25,
  k_REG_VPT       = 0x26,
  k_REG_REG28     = 0x28,
  k_REG_HOutSize  = 0x29,
  k_REG_EXHCH     = 0x2A,
  k_REG_EXHCL     = 0x2B,
  k_REG_VOutSize  = 0x2C,
  k_REG_ADVFL     = 0x2D,
  k_REG_ADVFH     = 0x2E,
  k_REG_YAVE      = 0x2F,
  k_REG_LumHTh    = 0x30,
  k_REG_LumLTh    = 0x31,
  k_REG_HREF      = 0x32,
  k_REG_DM_LNL    = 0x33,
  k_REG_DM_LNH    = 0x34,
  k_REG_ADoff_B   = 0x35,
  k_REG_ADoff_R   = 0x36,
  k_REG_ADoff_Gb  = 0x37,
  k_REG_ADoff_Gr  = 0x38,
  k_REG_Off_B     = 0x39,
  k_REG_Off_R     = 0x3A,
  k_REG_Off_Gb    = 0x3B,
  k_REG_Off_Gr    = 0x3C,
  k_REG_COM12     = 0x3D,
  k_REG_COM13     = 0x3E,
  k_REG_COM14     = 0x3F,
  k_REG_COM16     = 0x41,
  k_REG_TGT_B     = 0x42,
  k_REG_TGT_R     = 0x43,
  k_REG_TGT_Gb    = 0x44,
  k_REG_TGT_Gr    = 0x45,
  k_REG_LC_CTR    = 0x46,
  k_REG_LC_XC     = 0x47,
  k_REG_LC_YC     = 0x48,
  k_REG_LC_COEF   = 0x49,
  k_REG_LC_RADI   = 0x4A,
  k_REG_LC_COEFB  = 0x4B,
  k_REG_LC_COEFR  = 0x4C,
  k_REG_FixGain   = 0x4D,
  k_REG_AREF1     = 0x4F,
  k_REG_AREF6     = 0x54,
  k_REG_UFix      = 0x60,
  k_REG_VFix      = 0x61,
  k_REG_AWBb_blk  = 0x62,
  k_REG_AWB_Ctrl0 = 0x63,
  k_REG_DSP_Ctrl1 = 0x64,
  k_REG_DSP_Ctrl2 = 0x65,
  k_REG_DSP_Ctrl3 = 0x66,
  k_REG_DSP_Ctrl4 = 0x67,
  k_REG_AWB_bias  = 0x68,
  k_REG_AWBCtrl1  = 0x69,
  k_REG_AWBCtrl2  = 0x6A,
  k_REG_AWBCtrl3  = 0x6B,
  k_REG_AWBCtrl4  = 0x6C,
  k_REG_AWBCtrl5  = 0x6D,
  k_REG_AWBCtrl6  = 0x6E,
  k_REG_AWBCtrl7  = 0x6F,
  k_REG_AWBCtrl8  = 0x70,
  k_REG_AWBCtrl9  = 0x71,
  k_REG_AWBCtrl10 = 0x72,
  k_REG_AWBCtrl11 = 0x73,
  k_REG_AWBCtrl12 = 0x74,
  k_REG_AWBCtrl13 = 0x75,
  k_REG_AWBCtrl14 = 0x76,
  k_REG_AWBCtrl15 = 0x77,
  k_REG_AWBCtrl16 = 0x78,
  k_REG_AWBCtrl17 = 0x79,
  k_REG_AWBCtrl18 = 0x7A,
  k_REG_AWBCtrl19 = 0x7B,
  k_REG_AWBCtrl20 = 0x7C,
  k_REG_AWBCtrl21 = 0x7D,
  k_REG_GAM1      = 0x7E,
  k_REG_GAM2      = 0x7F,
  k_REG_GAM3      = 0x80,
  k_REG_GAM4      = 0x81,
  k_REG_GAM5      = 0x82,
  k_REG_GAM6      = 0x83,
  k_REG_GAM7      = 0x84,
  k_REG_GAM8      = 0x85,
  k_REG_GAM9      = 0x86,
  k_REG_GAM10     = 0x87,
  k_REG_GAM11     = 0x88,
  k_REG_GAM12     = 0x89,
  k_REG_GAM13     = 0x8A,
  k_REG_GAM14     = 0x8B,
  k_REG_GAM15     = 0x8C,
  k_REG_SLOP      = 0x8D,
  k_REG_DNSTh     = 0x8E,
  k_REG_EDGE0     = 0x8F,
  k_REG_EDGE1     = 0x90,
  k_REG_DNSOff    = 0x91,
  k_REG_EDGE2     = 0x92,
  k_REG_EDGE3     = 0x93,
  k_REG_MTX1      = 0x94,
  k_REG_MTX2      = 0x95,
  k_REG_MTX3      = 0x96,
  k_REG_MTX4      = 0x97,
  k_REG_MTX5      = 0x98,
  k_REG_MTX6      = 0x99,
  k_REG_MTX_Ctrl  = 0x9A,
  k_REG_BRIGHT    = 0x9B,
  k_REG_CNST      = 0x9C,
  k_REG_UVADJ0    = 0x9E,
  k_REG_UVADJ1    = 0x9F,
  k_REG_SCAL0     = 0xA0,
  k_REG_SCAL1     = 0xA1,
  k_REG_SCAL2     = 0xA2,
  k_REG_SDE       = 0xA6,
  k_REG_USAT      = 0xA7,
  k_REG_VSAT      = 0xA8,
  k_REG_HUECOS    = 0xA9,
  k_REG_HUESIN    = 0xAA,
  k_REG_SIGN      = 0xAB,
  k_REG_DSPAuto   = 0xAC,
} reg_name;

typedef enum {
  k_auto = 0,
  k_sunny,
  k_cloudy,
  k_office,
  k_home,
  k_night,
} light_mode;

typedef enum {
  k_normal = 0,
  k_balck_white,
  k_bluish,
  k_sepia,
  k_redish,
  k_greenish,
  k_negative,
} special_effects;

typedef enum {
  k_QVGA = 0,
  k_VGA,
} resolution_param;

/*摄像头配置结构体*/
typedef struct {
  resolution_param QVGA_VGA;  // 0：QVGA，1VGA

  /*VGA:sx + width <= 320 或 240 ,sy+height <= 320 或 240*/
  /*QVGA:sx + width <= 320 ,sy+height <= 240*/
  uint16_t cam_sx;  // 摄像头窗口X起始位置
  uint16_t cam_sy;  // 摄像头窗口Y起始位置

  uint16_t cam_width;   // 图像分辨率，宽
  uint16_t cam_height;  // 图像分辨率，高

  uint16_t lcd_sx;    // 图像显示在液晶屏的X起始位置
  uint16_t lcd_sy;    // 图像显示在液晶屏的Y起始位置
  uint8_t  lcd_scan;  // 液晶屏的扫描模式（0-7）

  uint8_t light_mode;  // 光照模式，参数范围[0~5]
  int8_t  saturation;  // 饱和度,参数范围[-4 ~ +4]
  int8_t  brightness;  // 光照度，参数范围[-4~+4]
  int8_t  contrast;    // 对比度，参数范围[-4~+4]
  uint8_t effect;      // 特殊效果，参数范围[0~6]:
} ov7725_mode_param_t;

bool Ov7725_init(void);
void ov7725_mode_config(void);
void ov7725_light_mode(light_mode mode);
void ov7725_color_saturation(int8_t sat);
void ov7725_brightness(int8_t brightness);
void ov7725_constrast(int8_t cnst);
void ov7725_special_effect(special_effects eff);
void ov7725_window_set(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height, resolution_param QVGA_VGA);

ov7725_mode_param_t get_camera_mode(void);
