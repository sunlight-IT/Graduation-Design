#include "ld3320_apply.h"

#include "component/component.h"

#define TAG "ld3320_apply"

static uint8_t            asr_resualt;
static bool               check_operate;
static voidce_cmd_code_t *voice_cache;

void modification(void) {
  asr_resualt = get_asr_result();
  if ((asr_code_num)asr_resualt == k_code_cmd) {
    check_operate = true;
    ZLOGI(TAG, "check_open sucess");
  }
  if (check_operate) {
    switch (asr_resualt) {
      case k_code_dmcs:
        ZLOGI(TAG, "%s", get_voice_cmd_data_chinese(k_code_dmcs));
        break;
      case k_code_cswb:
        ZLOGI(TAG, "%s", get_voice_cmd_data_chinese(k_code_cswb));
        break;
      case k_code_1:
        ZLOGI(TAG, "%s", get_voice_cmd_data_chinese(k_code_1));
        break;
      case k_code_2:
        break;
      case k_code_3:
        break;
      default:
        break;
    }
  } else {
    ZLOGI(TAG, "输入一级指令");
  }
}

void voice_apply(void) {
  switch (get_asr_status()) {
    case k_asr_runing:
      // ZLOGE(TAG, "RUNING_ASR");
      break;
    case k_asr_error:
      ZLOGE(TAG, "ERROR_ASR");
      break;
    case k_asr_none:
      if (!asr_recognize()) {
        set_asr_status(k_asr_error);
        ZLOGW(TAG, "asr_recognize error");
      }
      ZLOGW(TAG, "asr_recognize ok");
      set_asr_status(k_asr_runing);
      break;
    case k_asr_foundok:
      modification();
      set_asr_status(k_asr_none);
      break;
    case k_asr_foundzero:
      ZLOGE(TAG, "ZERO_ASR");
    default:
      set_asr_status(k_asr_none);
      break;
  }

  // ld3320_test();
}
