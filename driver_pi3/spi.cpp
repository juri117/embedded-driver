#include "spi.h"

#if BUILD_TARGET == TARGET_PI3
static const char* TAG = "spiUtils";

Semaphore spiSemaphore;

void spi_init() {}

spi_device_handle_t spi_add_device(int ss_pin) { 
  if(wiringPiSPISetup(0, 1 * 1000 * 1000)>0){
    return 0;
  }
  // failed
  return 0;
}

/*
 * spi writes and reads always at the same time, so tx, rx buffer have to be
 * same length while reading usually you will send a tx buffer filled with 0x00
 * with expected rx buff length thats why it is enough to put the semaphore just
 * here
 * @param: spi device handle
 * @param: tx buffer
 * @param: rx buffer
 * @param: buffers len
 */
error_t spi_send(spi_device_handle_t dev, uint8_t* txBuff, uint8_t* rxBuff,
                 uint16_t len) {
  wiringPiSPIDataRW(dev, (unsigned char *)txBuff, len);
  memcpy(rxBuff, txBuff, len);
  return ERROR_OK;
}

void setup_spi_ready_interrupt(int intr_pin) {
  pinMode(intr_pin, INPUT);
}
#endif