#include "sccb.h"

#include "component/component.h"

#define TAG "SCCB_BUS"

static void sccb_delay(void) {
  uint16_t i = 400;
  while (i) i--;
}

static bool sccb_start(void) {
  SDIO_D(1);
  SDIO_C(1);
  sccb_delay();
  SDIO_D(0);
  sccb_delay();
  SDIO_C(0);
  sccb_delay();
  return true;
}

static bool sccb_stop(void) {
  SDIO_C(0);
  sccb_delay();
  SDIO_D(0);
  sccb_delay();
  SDIO_C(1);
  sccb_delay();
  SDIO_D(1);
  sccb_delay();
}

static bool sccb_ack(void) {
  SDIO_C(0);
  sccb_delay();
  SDIO_D(0);
  sccb_delay();
  SDIO_C(1);
  sccb_delay();
  SDIO_C(0);
  sccb_delay();
}

static void sccb_no_ack(void) {
  SDIO_D(1);
  sccb_delay();
  SDIO_C(1);
  sccb_delay();
  SDIO_C(0);
  sccb_delay();
  SDIO_D(0);
  sccb_delay();
}

static bool sccb_wait_ack(void) {
  bool status;
  SDIO_C(0);
  sccb_delay();
  SDIO_D(1);
  sccb_delay();
  SDIO_C(1);
  sccb_delay();

  if (SDIO_D_READ)  //
  {
    status = false;
  } else {
    status = true;
  }
  SDIO_C(0);
  sccb_delay();
  return status;
}

static bool sccb_send_byte(uint8_t byte) {
  bool    status;
  uint8_t data = byte;
  for (int i = 8; i > 0; i--) {
    SDIO_C(0);
    sccb_delay();
    if (data & 0x80)
      SDIO_D(1);
    else
      SDIO_D(0);
    data <<= 1;
    sccb_delay();
    SDIO_C(1);
    sccb_delay();
  }
  SDIO_D_IN();
  sccb_delay();
  SDIO_C(0);
  sccb_delay();
  SDIO_C(1);
  sccb_delay();

  if (SDIO_D_READ) {
    status = false;
  } else {
    status = true;
  }
  SDIO_C(0);
  sccb_delay();
  SDIO_D_OUT();
  sccb_delay();
  return status;
}

static uint8_t sccb_receive_byte(void) {
  int i = 8;

  uint8_t receive_byte = 0;
  SDIO_D_IN();
  sccb_delay();
  for (; i > 0; i--) {
    receive_byte <<= 1;
    SDIO_C(0);
    sccb_delay();
    SDIO_C(1);
    sccb_delay();
    if (SDIO_D_READ) receive_byte |= 0x01;
    sccb_delay();
  }
  SDIO_C(0);
  sccb_delay();
  SDIO_D_OUT();
  return receive_byte;
}

bool sccb_write_byte(uint8_t write_addr, uint8_t byte) {
  if (!sccb_start()) {
    ErrorHanding(TAG, "start error");
    return false;
  }
  if (!sccb_send_byte(ADDR_OV7725)) {
    ErrorHanding(TAG, "sccb_send_byte error1");
    return false;
  }
  // if (!sccb_wait_ack()) {
  //   sccb_stop();
  //   ErrorHanding(TAG, "sccb_wait_ack error");
  //   return false;
  // }
  if (!sccb_send_byte(write_addr & 0x00ff)) {
    ErrorHanding(TAG, "sccb_send_byte error2");
    return false;
  }
  // sccb_wait_ack();
  if (!sccb_send_byte(byte)) {
    ErrorHanding(TAG, "sccb_send_byte error3");
    return false;
  }
  // sccb_wait_ack();
  sccb_stop();
  return true;
}

bool sccb_read_byte(uint8_t read_addr, uint8_t *read_buf, uint16_t len) {
  // 2 step write
  if (!sccb_start()) {
    ErrorHanding(TAG, "start error");
    return false;
  }
  if (!sccb_send_byte(ADDR_OV7725)) {
    ErrorHanding(TAG, "sccb_send_byte error1");

    return false;
  }
  // if (!sccb_wait_ack()) {
  //   sccb_stop();
  //   ErrorHanding(TAG, "sccb_wait_ack error");
  //   return false;
  // }
  if (!sccb_send_byte(read_addr)) {
    ErrorHanding(TAG, "sccb_send_byte error2");

    return false;
  }
  // sccb_delay();
  // sccb_delay();
  // sccb_wait_ack();
  sccb_stop();
  // sccb_delay();
  // sccb_delay();
  // 2 step read;
  if (!sccb_start()) {
    ErrorHanding(TAG, "start error");
    return false;
  }

  if (!sccb_send_byte(ADDR_OV7725 | 0x01)) {
    ErrorHanding(TAG, "sccb_send_byte error3");

    return false;
  }
  // if (!sccb_wait_ack()) {
  //   sccb_stop();
  //   ErrorHanding(TAG, "sccb_wait_ack error");
  //   return false;
  // }
  for (; len > 0; len--) {
    *read_buf = sccb_receive_byte();
    if (len == 1)
      sccb_no_ack();
    else {
      read_buf++;
      sccb_ack();
    }
    // read_buf++;如果len==1非法越界了
  }
  sccb_stop();
  return true;
}