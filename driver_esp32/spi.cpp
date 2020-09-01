#include "spi.hpp"

#if BUILD_TARGET == TARGET_ESP32

static const char* TAG = "spiUtils";

Semaphore spiSemaphore;

void spi_init(gpio_num_t miso_gpio, gpio_num_t mosi_gpio, gpio_num_t clk_gpio) {
  // spiSemaphore = Semaphore();

  spi_bus_config_t buscfg = {};
  buscfg.miso_io_num = miso_gpio;
  buscfg.mosi_io_num = mosi_gpio;
  buscfg.sclk_io_num = clk_gpio;
  buscfg.quadwp_io_num = -1;
  buscfg.quadhd_io_num = -1;
  buscfg.max_transfer_sz = 320 * 2 * 8;

  // Initialize the SPI bus
  error_t ret = spi_bus_initialize(HSPI_HOST, &buscfg, 1);
  ESP_ERROR_CHECK(ret);
}

spi_device_handle_t spi_add_device(int ss_pin) {
  spi_device_handle_t dev;
  spi_device_interface_config_t devcfg = {};
  devcfg.clock_speed_hz = 1 * 1000 * 1000;  // max supportet by DWM1001: 8 Mhz
  devcfg.mode = 0;
  devcfg.spics_io_num = ss_pin;
  devcfg.queue_size = 1;
  devcfg.cs_ena_pretrans = 1;
  devcfg.cs_ena_posttrans = 1;
  // just in case acquire semaphore here
  error_t ret = ERROR_FAIL;
  if (spiSemaphore.take(SPI_SEMAPHORE_MAX_WAIT_SETUP_MS)) {
    ret = spi_bus_add_device(HSPI_HOST, &devcfg, &dev);
    spiSemaphore.give();
  } else {
    log_w(TAG, "Warning: could not obtain semaphore (init)");
  }
  if (ret != ERROR_OK) {
    log_e(TAG, "spi add device failed: %d", ret);
  }
  return (dev);
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
  std::shared_ptr<uint8_t> rx_data(
      (uint8_t*)heap_caps_malloc(len, MALLOC_CAP_DMA), heap_caps_free);
  spi_transaction_t trans = {};
  memset(&trans, 0, sizeof(trans));
  trans.length = len * 8;
  trans.rxlength = len * 8;  // 0 means default: equals length (see obove)
  trans.tx_buffer = txBuff;
  trans.rx_buffer = rx_data.get();
  error_t ret = ERROR_FAIL;
  if (spiSemaphore.take(SPI_SEMAPHORE_MAX_WAIT_MS)) {
    ret = spi_device_transmit(dev, &trans);
    // error_t ret = spi_device_polling_transmit(dev, &trans);
    spiSemaphore.give();
    memcpy(rxBuff, rx_data.get(), len);
  } else {
    log_w(TAG, "Warning: could not obtain semaphore (init)");
  }
  if (ret != ERROR_OK) {
    log_e(TAG, "spi send failed: %d", ret);
  }
  return ret;
}

void setup_spi_ready_interrupt(int intr_pin) {
  gpio_config_t io_conf = {};
  io_conf.mode = GPIO_MODE_INPUT;
  io_conf.pin_bit_mask = 1ull << intr_pin;
  io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.intr_type = GPIO_INTR_POSEDGE;
  ESP_ERROR_CHECK(gpio_config(&io_conf));
  // gpio_config(&io_conf);
}

#endif