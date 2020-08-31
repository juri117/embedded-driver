/**
 * @file TcpConnection.h
 * @author Juri Bieler (juribieler@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_PI3_TCP_CONNECTION_H
#define DRIVER_DRIVER_PI3_TCP_CONNECTION_H

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>

#include "../error.h"
#include "log.h"
#include "task.h"

#define WIFI_SOCK_MODE_OFF 0x00
// #define WIFI_SOCK_MODE_STA 0x01     // wifi client, tcp server
// #define WIFI_SOCK_MODE_AP 0x02      // wifi AP, tcp server
#define WIFI_SOCK_MODE_STA_CL 0x03  // wifi client, tcp client
#define WIFI_SOCK_MODE_AP_CL \
  0x04  // wifi AP, tcp client (set first connecting wifi client as tcp server)

#define MAX_IP_LEN 15

#define SEND_FAIL_MAX_FOR_DISCONNECT 5

#define TCP_IN_BUFFER_LEN 256

class TcpConnector {
 private:
  uint8_t mode;
  uint8_t send_error_counter;
  int sock;

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
  void wait_for_client_connected();
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

#endif /* DRIVER_DRIVER_PI3_TCP_CONNECTION_H */
