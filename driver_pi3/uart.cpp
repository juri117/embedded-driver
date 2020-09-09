
#include "uart.hpp"
#if BUILD_TARGET == TARGET_PI3

static const char* TAG = "uart";

extern int errno;

int uart_fd[UART_COUNT];

void uart_init(uart_port_t uart_num, uart_name_t uart_name, gpio_num_t dummy1,
               int baudRate) {
  uart_fd[uart_num] = serialOpen(uart_name, baudRate);
  if (uart_fd[uart_num] < 0) {
    log_e(TAG, "uart(%d) initialization failed: %s", uart_num, strerror(errno));
  }
}

void uart_init_driver(uart_port_t uart_num, int rx_buffer_size,
                      int tx_buffer_size) {}

// void uart_enable_pattern_check(uart_port_t uartPort, char pattern) {}

int uart_read(uart_port_t uart_num, uint8_t* buff, uint32_t length,
              uint32_t timeout_ms) {
  uint32_t start = get_time_system_ms();
  if (uart_fd[uart_num] < 0) {
    return ERROR_FAIL;
  }
  int read_len = 0;
  while (read_len <= 0) {
    read_len = serialDataAvail(uart_fd[uart_num]);
    if (read_len > 0) {
      if (read_len > length) {
        read_len = length;
      }
      for (int i = 0; i < read_len; i++) {
        buff[i] = serialGetchar(uart_fd[uart_num]);
      }
      return read_len;
    }
    if (get_time_system_ms() - start >= timeout_ms) {
      return ERROR_TIMEOUT;
    } else {
      task_delay_ms(10);
    }
  }
  return read_len;
}

int uart_write(uart_port_t uart_num, const uint8_t* buff, uint32_t len) {
  if (uart_fd[uart_num] < 0) {
    return ERROR_FAIL;
  }
  for (int i = 0; i < len; i++) {
    serialPutchar(uart_fd[uart_num], buff[i]);
  }
  return len;
}

error_t uart_flush_rx(uart_port_t uart_num) {
  if (uart_fd[uart_num] < 0) {
    return ERROR_FAIL;
  }
  serialFlush(uart_fd[uart_num]);
  return ERROR_OK;
}
int uart_flush_tx(uart_port_t uart_num) {
  if (uart_fd[uart_num] < 0) {
    return ERROR_FAIL;
  }
  serialFlush(uart_fd[uart_num]);
  return ERROR_OK;
}

void uart_wait_for_tx_done(uart_port_t uart_num, uint32_t timeout_ms) {}

void uart_change_baudrate(uart_port_t uart_num, int baudRate) {
  // TODO
}

#endif