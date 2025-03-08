#pragma once

#define TABL_NUM 16

typedef void (*table_process_func)(void);

typedef enum {
  kIdle,
  kPicData,
  kTrace,
  kShow,
  // 功能处理
} ENUM_PROCESS_STATE;

typedef struct {
  ENUM_PROCESS_STATE state;
  table_process_func func;
} TableProcess_t;

void current_state_change(ENUM_PROCESS_STATE state);
void process_table(void);
void process_attach_table(TableProcess_t process);
