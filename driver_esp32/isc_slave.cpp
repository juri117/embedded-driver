

#include "isc_slave.hpp"

#if BUILD_TARGET == TARGET_ESP32

//#define SPI_UTILS_TAG "i2cUtils"

static const char *TAG = "i2cSlave";

#define ACK_CHECK_EN \
  0x1                /*!< 0x1: I2C master will check ack from slave (else 0x0)*/
#define ACK_VAL 0x0  /*!< I2C ack value */
#define NACK_VAL 0x1 /*!< I2C nack value */

#define I2C_SLAVE_TX_BUF_LEN 512
#define I2C_SLAVE_RX_BUF_LEN 512

/**
 * @brief initializes i2c as slave
 *
 * @param port
 * @param sda
 * @param scl
 * @param address
 * @return error_t
 */
error_t isc_slave_init(i2c_port_t port, gpio_num_t sda, gpio_num_t scl,
                       uint16_t address) {
  log_i(TAG, "isc_slave_init: Starting I2C slave at port %d.", port);
  i2c_config_t conf;
  conf.sda_io_num = sda;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_io_num = scl;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.mode = I2C_MODE_SLAVE;
  conf.slave.addr_10bit_en = 0;
  conf.slave.slave_addr = address;
  error_t err;
  err = i2c_param_config(port, &conf);
  if (err != ERROR_OK) {
    log_e(TAG, "isc_slave_init: failed to config i2c on port %d", port);
    return err;
  }
  err = i2c_driver_install(port, conf.mode, I2C_SLAVE_RX_BUF_LEN,
                           I2C_SLAVE_TX_BUF_LEN, 0);
  if (err != ERROR_OK) {
    log_e(TAG,
          "isc_slave_init: failed to install i2c driver on port %d in "
          "slave mode",
          port);
    return err;
  }
  err = i2c_set_timeout(port, 1048575);
  if (err != ERROR_OK) {
    log_e(TAG, "isc_slave_init: error on setting i2c timeout ret: %d", err);
    return err;
  }
  return ERROR_OK;
}

int isc_slave_read_bytes(i2c_port_t i2c_num, uint8_t *data_rd, uint16_t size,
                         uint32_t timeout_ms) {
  return i2c_slave_read_buffer(i2c_num, data_rd, size,
                               pdMS_TO_TICKS(timeout_ms));
}

error_t isc_reset_tx(i2c_port_t port) { return i2c_reset_tx_fifo(port); }

error_t isc_slave_write_bytes(i2c_port_t port, uint8_t *data, uint16_t size,
                              uint32_t timeout_ms) {
  i2c_slave_write_buffer(port, data, size, pdMS_TO_TICKS(timeout_ms));
  return ERROR_OK;
}

#endif