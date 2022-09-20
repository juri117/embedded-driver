

#include "isc.hpp"

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

int isc_fd[ISC_ADDRESS_COUNT];

extern int pi_fd;

/**
 * @brief prints a table of accessable slaves addresses to log
 *
 * @param port
 */
void isc_scan(i2c_port_t port) {
  log_d(TAG, "Scanning I2C bus, port: %d", port);

  uint8_t address;
  error_t result;
  printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
  printf("00:         ");
  char *filename = (char *)"/dev/i2c-1";
  int i2c_port = 3;
  if ((i2c_port = open(filename, O_RDWR)) < 0) {
    return;
  }
  for (address = 3; address < 0x78; address++) {
    result = isc_probe(i2c_port, address);

    if (address % 16 == 0) {
      printf("\n%.2x:", address);
    }
    if (result == ERROR_OK) {
      printf(" %.2x", address);
      // log_i(TAG, "i2c device found, address: %.2x", address);
    } else {
      printf(" --");
    }
  }
  printf("\n");
}

/**
 * @brief send a single byte to slave address to probe for ACK
 *
 * @param port
 * @param address
 * @return error_t
 */
error_t isc_probe(i2c_port_t port, uint8_t address) {
  int res;
  if (ioctl(port, I2C_SLAVE, address) < 0) {
    if (errno == EBUSY) {
      printf("UU ");
      return ERROR_FAIL;
    } else {
      fprintf(stderr,
              "Error: Could not set "
              "address to 0x%02x: %s\n",
              address, strerror(errno));
      return ERROR_FAIL;
    }
  }
  if ((address >= 0x30 && address <= 0x37) ||
      (address >= 0x50 && address <= 0x5F))
    res = i2c_read_byte(pi_fd, port);
  else
    res = i2c_write_byte(pi_fd, port, I2C_SMBUS_WRITE);
  if (res < 0) {
    return ERROR_FAIL;
  }
  return ERROR_OK;
}

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
  if (isc_fd[address] < 0) {
    if (!isc_master_connect_device(address)) {
      return ERROR_FAIL;
    }
  }
  for (uint16_t i = 0; i < len; i++) {
    int ret = i2c_read_byte(pi_fd, isc_fd[address]);
    if (ret >= 0) {
      buff[i] = ret;
    } else {
      return ERROR_FAIL;
    }
  }
  return ERROR_OK;
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
  if (isc_fd[address] < 0) {
    if (!isc_master_connect_device(address)) {
      return ERROR_FAIL;
    }
  }
  for (uint16_t i = 0; i < readBuffLen; i++) {
    int ret = i2c_read_byte_data(pi_fd, isc_fd[address], regAdd + i);
    if (ret >= 0) {
      readBuff[i] = ret;
    } else {
      return ERROR_FAIL;
    }
  }
  return ERROR_OK;
}

error_t isc_master_read_register16(i2c_port_t i2c_num, uint16_t address,
                                   uint16_t regAdd, uint8_t *readBuff,
                                   uint16_t readBuffLen) {
  if (isc_fd[address] < 0) {
    if (!isc_master_connect_device(address)) {
      return ERROR_FAIL;
    }
  }
  uint16_t result;
  char buffer1[2] = {0};
  buffer1[0] = regAdd >> 8;
  buffer1[1] = regAdd & 0x00FF;
  int length1 = 2;  //<<< Number of bytes to write
  i2c_write_device(pi_fd, isc_fd[address], buffer1, length1);
  if (i2c_read_device(pi_fd, isc_fd[address], (char *)readBuff, readBuffLen) ==
      readBuffLen) {
    return ERROR_OK;
  }
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
  if (isc_fd[address] < 0) {
    if (!isc_master_connect_device(address)) {
      return ERROR_FAIL;
    }
  }
  write(isc_fd[address], buff, len);

  // old write function using wiringPi
  /*
    for (uint16_t i = 0; i < len; i++) {
      if (wiringPiI2CWrite(isc_fd[address], buff[i]) < 0) {
        return ERROR_FAIL;
      }
    }
    */
  return ERROR_OK;
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
  if (isc_fd[address] < 0) {
    if (!isc_master_connect_device(address)) {
      return ERROR_FAIL;
    }
  }
  for (uint16_t i = 0; i < writeBuffLen; i++) {
    int ret =
        i2c_write_byte_data(pi_fd, isc_fd[address], regAdd + i, writeBuff[i]);
    if (ret < 0) {
      return ERROR_FAIL;
    }
  }
  return ERROR_OK;
}

int isc_master_write_register16(i2c_port_t i2c_num, uint16_t address,
                                uint16_t regAdd, uint8_t *writeBuff,
                                uint16_t writeBuffLen) {
  if (isc_fd[address] < 0) {
    if (!isc_master_connect_device(address)) {
      return ERROR_FAIL;
    }
  }
  uint8_t buffer[2 + writeBuffLen] = {0};
  buffer[0] = regAdd >> 8;
  buffer[1] = regAdd & 0x00FF;
  memcpy(buffer + 2, writeBuff, writeBuffLen);
  // buffer[2] = regAdd >> 8;
  // buffer[3] = regAdd & 0x00FF;
  uint16_t length = 2 + writeBuffLen;
  if (write(isc_fd[address], buffer, length) != length) {
    return ERROR_OK;
  }
  return ERROR_FAIL;

  /*
    for (uint16_t i = 0; i < writeBuffLen; i++) {
      int ret = wiringPiI2CWriteReg16(isc_fd[address], regAdd + i,
    writeBuff[i]); if (ret < 0) { return ERROR_FAIL;
      }
    }
    return ERROR_OK;
  */
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
  // int file_handler = wiringPiI2CSetup(port);
  // if (file_handler > 0) {
  //   isc_fd[port] = file_handler;
  //   return ERROR_OK;
  // }
  // log_e(TAG, "could not init i2c on port %d, returned: %d", port,
  // file_handler); return ERROR_FAIL;
  for (uint16_t i = 0; i < ISC_ADDRESS_COUNT; i++) {
    isc_fd[i] = -1;
  }
  return ERROR_OK;
}

error_t isc_master_connect_device(uint16_t address) {
  if (address > ISC_ADDRESS_COUNT) {
    log_e(TAG, "invalid i2c address: %d", address);
    return ERROR_FAIL;
  }
  int file_handler = i2c_open(pi_fd, 0, address, 0);
  if (file_handler > 0) {
    isc_fd[address] = file_handler;
    return ERROR_OK;
  }
  log_e(TAG, "could not init i2c on port %d, returned: %d", address,
        file_handler);
  return ERROR_FAIL;
}

error_t isc_reset_tx(i2c_port_t port) {
  // TODO:
  return ERROR_FAIL;
}

#endif