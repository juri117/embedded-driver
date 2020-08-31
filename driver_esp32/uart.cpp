
#include "uart.h"

#if BUILD_TARGET == TARGET_ESP32

void uart_init(uart_port_t uart_num, gpio_num_t txPin, gpio_num_t rxPin,
               int baudRate) {
  uart_config_t uart_config = {};
  uart_config.baud_rate = baudRate;
  uart_config.data_bits = UART_DATA_8_BITS;
  uart_config.parity = UART_PARITY_DISABLE;
  uart_config.stop_bits = UART_STOP_BITS_1;
  uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;

  uart_param_config((uart_port_t)uart_num, &uart_config);
  uart_set_pin((uart_port_t)uart_num, txPin, rxPin, UART_PIN_NO_CHANGE,
               UART_PIN_NO_CHANGE);
  // uart_driver_install(uartPort, buffSizeRX, buffSizeTX, 0, NULL, 0);
}

void uart_init_driver(uart_port_t uart_num, int rx_buffer_size,
                      int tx_buffer_size) {
  uart_driver_install((uart_port_t)uart_num, rx_buffer_size, tx_buffer_size, 0,
                      NULL, 0);
}

// void uart_enable_pattern_check(uart_port_t uartPort, char pattern) {
//     uart_enable_pattern_det_intr(uartPort, pattern, 1, 10000, 10, 10);
// }

int uart_read(uart_port_t uart_num, uint8_t* buf, uint32_t length,
              uint32_t timeout_ms) {
  uint32_t start = get_time_system_ms();
  uint16_t len = 0;
  int res =
      uart_read_bytes((uart_port_t)uart_num, buf, 1, pdMS_TO_TICKS(timeout_ms));
  if (res <= 0) {
    return res;
  }
  len++;
  while (res > 0 && get_time_system_ms() - start < timeout_ms && len < length) {
    res =
        uart_read_bytes((uart_port_t)uart_num, buf + len, 1, pdMS_TO_TICKS(1));
    if (res > 0) {
      len += res;
    }
  }
  return len;
}

int uart_write(uart_port_t uart_num, const uint8_t* buff, uint32_t len) {
  return uart_write_bytes(uart_num, (const char*)buff, len);
}

error_t uart_flush_rx(uart_port_t uart_num) {
  return uart_flush_input((uart_port_t)uart_num);
}
error_t uart_flush_tx(uart_port_t uart_num) {
  return uart_flush((uart_port_t)uart_num);
}

void uart_wait_for_tx_done(uart_port_t uart_num, uint32_t timeout_ms) {
  uart_wait_tx_done((uart_port_t)uart_num, pdMS_TO_TICKS(timeout_ms));
}

void uart_change_baudrate(uart_port_t uart_num, int baudRate) {
  uart_set_baudrate(uart_num, baudRate);
}

#endif