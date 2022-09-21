
#include "uart.hpp"
#if BUILD_TARGET == TARGET_PI3

static const char* TAG = "uart";

extern int errno;
extern int pi_fd;

pi_uart_t uart_dev[UART_COUNT];
// int uart_fd[UART_COUNT];

void uart_init(uart_port_t uart_num, uart_name_t uart_name, gpio_num_t dummy1,
               int baudRate) {
  uart_dev[uart_num].fd = open(uart_name, O_RDWR | O_NOCTTY | O_NDELAY);
  // serial_open(pi_fd, (char*)uart_name, baudRate, 0);
  if (uart_dev[uart_num].fd < 0) {
    log_e(TAG, "uart(%d) initialization failed: %s", uart_num, strerror(errno));
    return;
  }
  struct termios options;
  tcgetattr(uart_dev[uart_num].fd, &options);
  int baud_flag = B115200;
  switch (baudRate) {
    case 9600:
      baud_flag = B9600;
      break;
    case 38400:
      baud_flag = B38400;
      break;
    case 57600:
      baud_flag = B57600;
      break;
    case 921600:
      baud_flag = B921600;
      break;
    default:
      baud_flag = B115200;
      break;
  }
  options.c_cflag = baud_flag | CS8 | CLOCAL | CREAD;  //<Set baud rate
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(uart_dev[uart_num].fd, TCIFLUSH);
  tcsetattr(uart_dev[uart_num].fd, TCSANOW, &options);
  uart_dev[uart_num].name = std::string(uart_name);
}

void uart_init_driver(uart_port_t uart_num, int rx_buffer_size,
                      int tx_buffer_size) {}

// void uart_enable_pattern_check(uart_port_t uartPort, char pattern) {}

int uart_read(uart_port_t uart_num, uint8_t* buff, uint32_t length,
              uint32_t timeout_ms) {
  uint32_t start = get_time_system_ms();
  if (uart_dev[uart_num].fd < 0) {
    task_delay_ms(timeout_ms);
    return ERROR_FAIL;
  }
  int read_len = 0;
  while (read_len <= 0) {
    // read_len = serial_data_available(pi_fd, uart_dev[uart_num].fd);
    // if (read_len > 0) {
    // if (read_len > length) {
    //  read_len = length;
    //}
    // for (int i = 0; i < read_len; i++) {
    //   buff[i] = serial_read_byte(pi_fd, uart_dev[uart_num].fd);
    // }
    read_len = read(uart_dev[uart_num].fd, buff, length);
    if (read_len > 0) return read_len;
    if (get_time_system_ms() - start >= timeout_ms) {
      return ERROR_TIMEOUT;
    } else {
      task_delay_ms(10);
    }
  }
  // else if (read_len < 0) {
  //   printf(TAG, "serial ERROR: %s\n", strerror(errno));
  // }

  //}
  return read_len;
}

int uart_write(uart_port_t uart_num, const uint8_t* buff, uint32_t len) {
  if (uart_dev[uart_num].fd < 0) {
    return ERROR_FAIL;
  }
  // for (int i = 0; i < len; i++) {
  write(uart_dev[uart_num].fd, buff, len);
  //}
  return len;
}

error_t uart_flush_rx(uart_port_t uart_num) {
  if (uart_dev[uart_num].fd < 0) {
    return ERROR_FAIL;
  }
  // serialFlush(uart_dev[uart_num].fd);
  return ERROR_OK;
}
int uart_flush_tx(uart_port_t uart_num) {
  if (uart_dev[uart_num].fd < 0) {
    return ERROR_FAIL;
  }
  // serialFlush(uart_dev[uart_num].fd);
  return ERROR_OK;
}

void uart_wait_for_tx_done(uart_port_t uart_num, uint32_t timeout_ms) {}

void uart_change_baudrate(uart_port_t uart_num, int baudRate) {
  close(uart_dev[uart_num].fd);
  uart_init(uart_num, uart_dev[uart_num].name.c_str(), 0, baudRate);
}

#endif