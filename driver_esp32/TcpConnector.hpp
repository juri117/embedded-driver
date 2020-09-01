/**
 * @file TcpConnection.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_ESP32_TCP_CONNECTION_H
#define DRIVER_DRIVER_ESP32_TCP_CONNECTION_H

#include <driver/gpio.h>
#include <driver/timer.h>
#include <driver/uart.h>
#include <esp_err.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>
#include <lwip/err.h>
#include <lwip/inet.h>
#include <lwip/init.h>
#include <lwip/ip4_addr.h>
#include <lwip/ip6_addr.h>
#include <lwip/ip_addr.h>
#include <lwip/netdb.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <mdns.h>
#include <nvs_flash.h>
#include <soc/soc.h>
#include <sys/param.h>

#include <string>

#include "../error.hpp"
#include "log.hpp"
#include "task.hpp"

#define WIFI_SOCK_MODE_OFF 0x00
// #define WIFI_SOCK_MODE_STA 0x01     // wifi client, tcp server
// #define WIFI_SOCK_MODE_AP 0x02      // wifi AP, tcp server
#define WIFI_SOCK_MODE_STA_CL 0x03  // wifi client, tcp client
#define WIFI_SOCK_MODE_AP_CL \
  0x04  // wifi AP, tcp client (set first connecting wifi client as tcp server)

#define MAX_IP_LEN 15

#define SEND_FAIL_MAX_FOR_DISCONNECT 10

#define TCP_IN_BUFFER_LEN 256

class TcpConnector {
 private:
  uint8_t mode;
  uint8_t send_error_counter;

 public:
  bool tcp_connected;
  bool wifi_connected;
  uint8_t ipAddress[MAX_IP_LEN];
  uint8_t ipAddressLen;

  uint8_t ipAddressClient[MAX_IP_LEN];
  uint8_t ipAddressClientLen;
  TcpConnector();
  void init(uint8_t mode, const uint8_t* ssid, const uint8_t* pw);
  void init_as_sta(const uint8_t* ssid, const uint8_t* pw);
  void init_as_ap(const uint8_t* ssid, const uint8_t* pw);
  void wait_for_startup();
  bool wait_for_client_connected();
  bool connect_to_server(const uint8_t* host_ip, uint32_t host_port);
  bool socket_is_open();
  void disconnect_from_server();
  int read_bytes(uint8_t* rx_buffer, uint16_t read_len);
  int write_bytes(uint8_t* bytes, uint16_t len);
  bool is_connected();
  bool is_wifi_connected();
  bool is_tcp_connected();
  uint8_t get_mode();
  uint8_t get_ip(uint8_t* ip);
};

#endif /* DRIVER_DRIVER_ESP32_TCP_CONNECTION_H */
