#ifndef _TIME_SLICE_H_
#define _TIME_SLICE_H_

#include "stdbool.h"
#include "stdint.h"

typedef enum {
  kImageData,
  kImageShow,
  kServoCrtl,
} ENUM_TIME_TYPE;

typedef struct {
  ENUM_TIME_TYPE type;
  uint32_t       timeimg_num;
  bool           state;
} TIME_DATA_T;

void apply_slice(void);

#endif