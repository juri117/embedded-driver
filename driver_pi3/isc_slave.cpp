

#include "isc_slave.hpp"

#if BUILD_TARGET == TARGET_PI3

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
  return ERROR_FAIL;
}

int isc_slave_read_bytes(i2c_port_t i2c_num, uint8_t *data_rd, uint16_t size,
                         uint32_t timeout_ms) {
  return -1;
}

error_t isc_slave_write_bytes(i2c_port_t port, uint8_t *data, uint16_t size,
                              uint32_t timeout_ms) {
  return ERROR_FAIL;
}

#endif