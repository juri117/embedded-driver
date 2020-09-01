#include "TcpConnector.hpp"

#if BUILD_TARGET == TARGET_PI3

static const char* TAG = "TcpConnector";

extern int errno;

#define AP_AUTHMODE \
  WIFI_AUTH_WPA2_PSK  // the passpharese should be atleast 8 chars long
#define AP_SSID_HIDDEN 0
#define AP_MAX_CONNECTIONS 1
#define AP_BEACON_INTERVAL 100  // in milli seconds

#define CONFIG_IPV4

int server_sock;

bool connected;
// bool tcp_connected;

static TaskEventVar tcp_event_group;

const int IPV4_GOTIP_BIT = BIT0;
const int IPV6_GOTIP_BIT = BIT1;
const int CLIENT_CONNECTED_BIT = BIT2;
const int CLIENT_DISCONNECTED_BIT = BIT3;
const int AP_STARTED_BIT = BIT4;
const int REC_MSG_AVAIL = BIT6;

const uint16_t InternetReconnectTryPause = 1000;

extern TcpConnector tcp;

TcpConnector::TcpConnector()
    : mode(0),
      send_error_counter(0),
      sock(0),
      tcp_connected(false),
      wifi_connected(true),
      ipAddress(),
      ipAddressLen(0),
      ipAddressClient(),
      ipAddressClientLen(0) {}

void TcpConnector::init(uint8_t mode, const uint8_t* ssid, const uint8_t* pw) {
  this->mode = mode;
}

void TcpConnector::init_as_sta(const uint8_t* ssid, const uint8_t* pw) {}

void TcpConnector::init_as_ap(const uint8_t* ssid, const uint8_t* pw) {}

void TcpConnector::wait_for_startup() {}

void TcpConnector::wait_for_client_connected() {}

bool TcpConnector::connect_to_server(const uint8_t* host_ip,
                                     uint32_t host_port) {
  int valread;
  struct sockaddr_in serv_addr;
  if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    log_e(TAG, "Socket creation error");
    return false;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(host_port);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, (const char*)host_ip, &serv_addr.sin_addr) <= 0) {
    log_e(TAG, "Invalid address/address not supported");
    return false;
  }

  if (connect(this->sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <
      0) {
    log_e(TAG, "Socket connection failed");
    return false;
  }
  this->tcp_connected = true;
  return true;
}

bool TcpConnector::socket_is_open() {
  return this->tcp_connected && this->sock > 0;
}

void TcpConnector::disconnect_from_server() {
  shutdown(this->sock, SHUT_RDWR);
  this->sock = 0;
  this->tcp_connected = false;
}

int TcpConnector::read_bytes(uint8_t* rx_buffer, uint16_t read_len) {
  if (this->socket_is_open()) {
    return recv(this->sock, rx_buffer, read_len, MSG_DONTWAIT);
  }
  return -1;
}

int TcpConnector::write_bytes(uint8_t* bytes, uint16_t len) {
  if (this->socket_is_open()) {
    int err = -1;
    try {
      err = send(this->sock, bytes, len, MSG_NOSIGNAL);
    } catch (...) {
    }
    // printf("TcpConnector: send cnt: %d, err: %d, cmd: %x\n",
    //        bytes[USR_CMD_COUNT_BYTE_OFFSET], err,
    //        bytes[USR_CMD_CMD_BYTE_OFFSET]);
    if (err < 0) {
      printf("TcpConnector: Error occured during sending: errno %s, ret %d\n",
             strerror(errno), err);
      this->send_error_counter++;
      if (this->send_error_counter >= SEND_FAIL_MAX_FOR_DISCONNECT) {
        printf("TcpConnector: hit send fail max, disconnect!\n");
        this->disconnect_from_server();
        // tcp_connected = false;
        this->send_error_counter = 0;
      }
    } else {
      this->send_error_counter = 0;
    }
    return err;
  }
  return -1;
}

bool TcpConnector::is_connected() { return server_sock > 0; }
bool TcpConnector::is_wifi_connected() { return this->wifi_connected; }
bool TcpConnector::is_tcp_connected() { return this->tcp_connected; }

uint8_t TcpConnector::get_mode() { return this->mode; }

uint8_t TcpConnector::get_ip(uint8_t* ip) {
  memcpy(ip, this->ipAddress, this->ipAddressLen);
  return this->ipAddressLen;
}

#endif