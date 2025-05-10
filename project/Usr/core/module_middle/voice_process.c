#include "voice_process.h"

#include "Core/module_driver/voice_driver/syn6288_driver/syn6288_driver.h"
#include "component/component.h"

static uint8_t syn_status;
void           fsm_voice_process(void) { voice_inquire_cmd(); }