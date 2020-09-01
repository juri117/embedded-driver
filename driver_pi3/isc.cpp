

#include "isc.h"

#if BUILD_TARGET == TARGET_PI3

//#define SPI_UTILS_TAG "i2cUtils"

static const char *TAG = "i2c";

#define I2C_MASTER_FREQ_HZ 100000 /*!< I2C master clock frequency */
#define ACK_CHECK_EN \
  0x1                /*!< 0x1: I2C master will check ack from slave (else 0x0)*/
#define ACK_VAL 0x0  /*!< I2C ack value */
#define NACK_VAL 0x1 /*!< I2C nack value */

// #define WRITE_BIT I2C_MASTER_WRITE /*!< I2C master write */
// #define READ_BIT I2C_MASTER_READ   /*!< I2C master read */

#define I2C_SLAVE_TX_BUF_LEN 512
#define I2C_SLAVE_RX_BUF_LEN 512

#define I2C_MASTER_TX_BUF_LEN 512
#define I2C_MASTER_RX_BUF_LEN 512

/**
 * @brief prints a table of accessable slaves addresses to log
 *
 * @param port
 */
void isc_scan(i2c_port_t port) {
  log_d(TAG, "Scanning I2C bus.");

  uint8_t address;
  error_t result;
  // printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
  // printf("00:         ");
  for (address = 3; address < 0x78; address++) {
    result = isc_probe(port, address);

    // if (address % 16 == 0) {
    //   printf("\n%.2x:", address);
    // }
    if (result == ERROR_OK) {
      // printf(" %.2x", address);
      log_i(TAG, "i2c device found, address: %.2x", address);
    }
    //  else {
    //   printf(" --");
    // }
  }
  // printf("\n");
}

/**
 * @brief send a single byte to slave address to probe for ACK
 *
 * @param port
 * @param address
 * @return error_t
 */
error_t isc_probe(i2c_port_t port, uint8_t address) { return -1; }

/**
 * @brief this function reads bytes from a slave
 *
 * @param i2c_num
 * @param address
 * @param buff
 * @param len
 * @return error_t
 */
error_t isc_master_read_bytes(i2c_port_t i2c_num, uint16_t address,
                              uint8_t *buff, uint16_t len) {
  // TODO:
  return ERROR_FAIL;
}

/**
 * @brief read from slave, giving a specific register
 *
 * @param i2c_num
 * @param address
 * @param regAdd
 * @param readBuff
 * @param readBuffLen
 * @return error_t
 */
error_t isc_master_read_register(i2c_port_t i2c_num, uint16_t address,
                                 uint8_t regAdd, uint8_t *readBuff,
                                 uint16_t readBuffLen) {
  // TODO:
  return ERROR_FAIL;
}

/**
 * @brief writes to slave
 *
 * @param i2c_num
 * @param address
 * @param buff
 * @param len
 * @return int
 */
int isc_master_write(i2c_port_t i2c_num, uint16_t address, uint8_t *buff,
                     uint16_t len) {
  // TODO:
  return ERROR_FAIL;
}

/**
 * @brief writes to slave in a specific register
 *
 * @param i2c_num
 * @param address
 * @param regAdd
 * @param writeBuff
 * @param writeBuffLen
 * @return int
 */
int isc_master_write_register(i2c_port_t i2c_num, uint16_t address,
                              uint8_t regAdd, uint8_t *writeBuff,
                              uint16_t writeBuffLen) {
  // TODO:
  return ERROR_FAIL;
}

/**
 * @brief initializes i2c as master
 *
 * @param port
 * @param sda
 * @param scl
 * @return error_t
 */
error_t isc_master_init(i2c_port_t port, gpio_num_t sda, gpio_num_t scl) {
  // TODO:
  return ERROR_FAIL;
}

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
  // TODO:
  return ERROR_FAIL;
}

int isc_slave_read_bytes(i2c_port_t i2c_num, uint8_t *data_rd, uint16_t size,
                         uint32_t timeout_ms) {
  // TODO:
  return 0;
}

error_t isc_reset_tx(i2c_port_t port) {
  // TODO:
  return ERROR_FAIL;
}

error_t isc_slave_write_bytes(i2c_port_t port, uint8_t *data, uint16_t size,
                              uint32_t timeout_ms) {
  // TODO:
  return ERROR_FAIL;
}

#endif