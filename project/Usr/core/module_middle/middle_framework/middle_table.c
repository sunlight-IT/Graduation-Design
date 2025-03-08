#include "middle_table.h"

#include <stdint.h>
#include <string.h>

#include "component/log/my_log.h"

static TableProcess_t     table_process[TABL_NUM];
static ENUM_PROCESS_STATE current_state;
static uint8_t            table_num;

void process_attach_table(TableProcess_t process) {
  if (table_num >= TABL_NUM) {
    LOGE("table overflow %d", table_num);
    return;
  }
  table_process[table_num] = process;
}

void current_state_reset(void) { current_state = kIdle; }

void current_state_change(ENUM_PROCESS_STATE state) {  //

  current_state = state;
}

void process_table(void) {
  for (int i = 0; i < table_num; i++) {
    if (table_process[i].func != NULL && table_process[i].state == current_state) {
      table_process[i].func();
      state_reset();
    }
  }
}