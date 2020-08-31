

#include "isc.h"

#if BUILD_TARGET == TARGET_ESP32

//#define SPI_UTILS_TAG "i2cUtils"

static const char *TAG = "i2cUtils";

#define I2C_MASTER_FREQ_HZ 100000 /*!< I2C master clock frequency */
#define ACK_CHECK_EN \
  0x1                /*!< 0x1: I2C master will check ack from slave (else 0x0)*/
#define ACK_VAL 0x0  /*!< I2C ack value */
#define NACK_VAL 0x1 /*!< I2C nack value */

#define WRITE_BIT I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ   /*!< I2C master read */

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
error_t isc_probe(i2c_port_t port, uint8_t address) {
    error_t result;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE,
        1 /* expect ack */);
    i2c_master_stop(cmd);
    result = i2c_master_cmd_begin(port, cmd, 10 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return result;
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
    error_t err;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    err = i2c_master_start(cmd);
    if (err != ERROR_OK) {
        log_e(TAG,
            "isc_master_read_bytes: could not start write connection, err: %d",
            err);
        return -1;
    }
    err = i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_READ,
        ISC_ACK_CHECK_ENABLE);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_read_bytes: could not write start byte, err: %d",
            err);
        return -1;
    }
    if (len > 1) {
        err = i2c_master_read(cmd, buff, len - 1, (i2c_ack_type_t)ISC_ACK_VAL);
        if (err != ERROR_OK) {
            log_e(TAG, "isc_master_read_bytes: could not read bytes, err: %d", err);
            return -1;
        }
    }
    err = i2c_master_read_byte(cmd, buff + len - 1, (i2c_ack_type_t)ISC_NACK_VAL);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_read_bytes: could not read nak byte, err: %d", err);
        return -1;
    }
    err = i2c_master_stop(cmd);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_read_bytes: could not stop, err: %d", err);
        return -1;
    }
    err = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    if (err != ERROR_OK) {
        // log_e(TAG, "isc_master_read_bytes: fail on begin, err: %d", err);
        return -1;
    }
    i2c_cmd_link_delete(cmd);
    return err;
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
    error_t err;
    if (readBuffLen == 0) {
        return ERROR_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    err = i2c_master_start(cmd);
    if (err != ERROR_OK) {
        log_e(TAG,
            "isc_master_read_register: could not start write connection, err: %d",
            err);
        return -1;
    }
    // first, send device address (indicating write) & register to be read
    err = i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE,
        ISC_ACK_CHECK_ENABLE);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_read_register: could not write start byte, err: %d",
            err);
        return -1;
    }
    // send register we want
    err = i2c_master_write_byte(cmd, regAdd, ISC_ACK_CHECK_ENABLE);
    if (err != ERROR_OK) {
        log_e(TAG,
            "isc_master_read_register: could not write register address byte, "
            "err: %d",
            err);
        return -1;
    }
    // Send repeated start
    err = i2c_master_start(cmd);
    if (err != ERROR_OK) {
        log_e(TAG,
            "isc_master_read_register: could not start v2 write connection, "
            "err: %d",
            err);
        return -1;
    }
    // now send device address (indicating read) & read data
    err = i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_READ,
        ISC_ACK_CHECK_ENABLE);
    if (err != ERROR_OK) {
        log_e(TAG,
            "isc_master_read_register: could not write start byte v2, err: %d",
            err);
        return -1;
    }
    if (readBuffLen > 1) {
        err = i2c_master_read(cmd, readBuff, readBuffLen - 1,
            (i2c_ack_type_t)ISC_ACK_VAL);
        if (err != ERROR_OK) {
            log_e(TAG, "isc_master_read_register: could not read bytes, err: %d",
                err);
            return -1;
        }
    }
    err = i2c_master_read_byte(cmd, readBuff + readBuffLen - 1,
        (i2c_ack_type_t)ISC_NACK_VAL);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_read_register: could not read nak byte, err: %d",
            err);
        return -1;
    }
    err = i2c_master_stop(cmd);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_read_register: could not stop, err: %d", err);
        return -1;
    }
    err = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_read_register: fail on begin, err: %d", err);
        return -1;
    }
    i2c_cmd_link_delete(cmd);
    return err;
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
    error_t err;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    err = i2c_master_start(cmd);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_write: could not start write connection, err: %d",
            err);
        return -1;
    }
    err = i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE,
        ISC_ACK_CHECK_ENABLE);
    if (err != ERROR_OK) {
        log_e(TAG,
            "isc_master_write: could not write request for write connection, "
            "err: %d",
            err);
        return -1;
    }
    err = i2c_master_write(cmd, buff, len, ISC_ACK_CHECK_ENABLE);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_write: could not write bytes, err: %d", err);
        return -1;
    }
    err = i2c_master_stop(cmd);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_write: could not stop, err: %d", err);
        return -1;
    }
    err = i2c_master_cmd_begin(i2c_num, cmd, pdMS_TO_TICKS(100));
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_write: fail on begin, err: %d", err);
    }
    i2c_cmd_link_delete(cmd);
    return err;
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
    error_t err;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    err = i2c_master_start(cmd);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_write_register: could not start connection, err: %d",
            err);
        return -1;
    }
    // first, send device address (indicating write) & register to be written
    err = i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE,
        ISC_ACK_CHECK_ENABLE);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_write_register: could not write start byte, err: %d",
            err);
        return -1;
    }
    // send register we want
    err = i2c_master_write_byte(cmd, regAdd, ISC_ACK_CHECK_ENABLE);
    if (err != ERROR_OK) {
        log_e(TAG,
            "isc_master_write_register: could not write register byte, err: %d",
            err);
        return -1;
    }
    // write the data
    err = i2c_master_write(cmd, writeBuff, writeBuffLen, ISC_ACK_CHECK_ENABLE);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_write_register: could not write bytes, err: %d",
            err);
        return -1;
    }
    err = i2c_master_stop(cmd);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_write_register: could not stop, err: %d", err);
        return -1;
    }
    err = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_write_register: fail on begin, err: %d", err);
    }
    i2c_cmd_link_delete(cmd);
    return err;
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
    log_d(TAG, "isc_master_init: Starting I2C master at port %d.", port);
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = sda;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = scl;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    error_t err = i2c_param_config(port, &conf);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_init: error on setting i2c params ret: %d", err);
        return err;
    }
    err = i2c_driver_install(port, conf.mode, I2C_MASTER_RX_BUF_LEN,
        I2C_MASTER_TX_BUF_LEN, 0);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_init: error on installing i2c driver ret: %d", err);
        return err;
    }
    err = i2c_set_timeout(port, 1048575);
    if (err != ERROR_OK) {
        log_e(TAG, "isc_master_init: error on setting i2c timeout ret: %d", err);
        return err;
    }
    return ERROR_OK;
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

int isc_slave_read_bytes(i2c_port_t i2c_num,
    uint8_t *data_rd, uint16_t size, uint32_t timeout_ms) {
    return i2c_slave_read_buffer(i2c_num, data_rd,
        size, pdMS_TO_TICKS(timeout_ms));
}

error_t isc_reset_tx(i2c_port_t port) {
    return i2c_reset_tx_fifo(port);
}

error_t isc_slave_write_bytes(i2c_port_t port,
    uint8_t *data, uint16_t size, uint32_t timeout_ms) {
    i2c_slave_write_buffer(port, data, size,
        pdMS_TO_TICKS(timeout_ms));
    return ERROR_OK;
}

#endif