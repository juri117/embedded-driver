#include "TcpConnector.hpp"

#if BUILD_TARGET == TARGET_ESP32

static const char* TAG = "TcpConnector";

extern int errno;

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

static void wifi_event_handler_sta(void* arg, esp_event_base_t event_base,
                                   int32_t event_id, void* event_data) {
  log_i(TAG, "sta event: base %s, id %d", event_base, event_id);
  if (event_base == WIFI_EVENT) {
    switch (event_id) {
      case WIFI_EVENT_STA_START: {
        esp_wifi_connect();
        log_i(TAG, "SYSTEM_EVENT_STA_START");
        break;
      }
      case WIFI_EVENT_STA_CONNECTED: {
        /* enable ipv6 */
        log_i(TAG, "WIFI_EVENT_STA_CONNECTED");
        tcpip_adapter_create_ip6_linklocal(TCPIP_ADAPTER_IF_STA);
        break;
      }
      case WIFI_EVENT_STA_DISCONNECTED: {
        log_i(TAG, "WIFI_EVENT_STA_DISCONNECTED");
        esp_wifi_connect();
        tcp_event_group.clear(IPV4_GOTIP_BIT);
        tcp_event_group.clear(IPV6_GOTIP_BIT);
        break;
      }
    }
  }
  if (event_base == IP_EVENT) {
    switch (event_id) {
      case IP_EVENT_STA_GOT_IP: {
        tcp_event_group.set(IPV4_GOTIP_BIT);
        ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
        memset(tcp.ipAddress, 0x00, MAX_IP_LEN);
        char* ip4 = esp_ip4addr_ntoa(&event->ip_info.ip, (char*)tcp.ipAddress,
                                     MAX_IP_LEN);
        // store length of the ip address since it comes without leading zeroes
        while (tcp.ipAddressLen < MAX_IP_LEN) {
          if (tcp.ipAddress[tcp.ipAddressLen] == 0x00) {
            break;
          }
          tcp.ipAddressLen++;
        }
        log_i(TAG, "IPv4: %s", ip4);
        break;
      }
      case IP_EVENT_GOT_IP6: {
        tcp_event_group.set(IPV6_GOTIP_BIT);
        // TODO: take this back in ?
        // ip_event_got_ip6_t* event_v6 = (ip_event_got_ip6_t*)event_data;
        log_i(TAG, "SYSTEM_EVENT_STA_GOT_IP6");
        // char* ip6 = esp_ip6addr_ntoa(&event->ip6_info.ip);
        // log_i(TAG, "IPv6: %s", ip6);
        break;
      }
      default:
        break;
    }
  }
}

static void wifi_event_handler_ap(void* arg, esp_event_base_t event_base,
                                  int32_t event_id, void* event_data) {
  log_i(TAG, "sta event: base %s, id %d", event_base, event_id);
  if (event_base == WIFI_EVENT) {
    switch (event_id) {
      case WIFI_EVENT_AP_START:
        log_i(TAG, "Event:ESP32 is started in AP mode\n");
        // xEventGroupSetBits(tcp_event_group, AP_STARTED_BIT);
        tcp_event_group.set(AP_STARTED_BIT);
        break;
      case WIFI_EVENT_AP_STACONNECTED: {
        log_i(TAG, "connect detected");
        // seems to be necessary, otherwise the clients IP returns 0.0.0.0 (only
        // on first connect after esp reboot)
        task_delay_ms(1000);
        tcp.wifi_connected = true;
        wifi_sta_list_t wifi_sta_list = {};
        memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
        error_t res = esp_wifi_ap_get_sta_list(&wifi_sta_list);
        if (res != ERROR_OK) {
          log_e(TAG, "wifi_event_handler_ap: could not get clients list");
          return;
        }
        tcpip_adapter_sta_list_t adapter_sta_list;
        memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
        res = tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);
        if (res != ERROR_OK) {
          log_e(TAG, "wifi_event_handler_ap: could not get clients ip");
          return;
        }
        if (adapter_sta_list.num > 0) {
          const esp_ip4_addr_t client_ip = adapter_sta_list.sta[0].ip;
          memset(tcp.ipAddressClient, 0x00, MAX_IP_LEN);
          // char* ip4 =
          esp_ip4addr_ntoa(&client_ip, (char*)tcp.ipAddressClient, MAX_IP_LEN);
          while (tcp.ipAddressClientLen < MAX_IP_LEN) {
            if (tcp.ipAddressClient[tcp.ipAddressClientLen] == 0x00) {
              break;
            }
            tcp.ipAddressClientLen++;
          }
          // memcpy(tcp.ipAddressClient, ip4, tcp.ipAddressClientLen);
          log_i(TAG, "clients(count: %d) IPv4: %s", adapter_sta_list.num,
                tcp.ipAddressClient);
        }
        tcp_event_group.set(CLIENT_CONNECTED_BIT);
      } break;
      case WIFI_EVENT_AP_STADISCONNECTED:
        // xEventGroupSetBits(tcp_event_group, CLIENT_DISCONNECTED_BIT);
        tcp_event_group.set(CLIENT_DISCONNECTED_BIT);
        log_i(TAG, "disconnect detected");
        tcp.wifi_connected = false;
        tcp.tcp_connected = false;
        break;
      default:
        break;
    }
  }
}

TcpConnector::TcpConnector()
    : mode(0),
      send_error_counter(0),
      tcp_connected(false),
      wifi_connected(false),
      ipAddress(),
      ipAddressLen(0),
      ipAddressClient(),
      ipAddressClientLen(0) {
  log_set_level("wifi", LOG_WARN);
}

void TcpConnector::init(uint8_t mode, const uint8_t* ssid, const uint8_t* pw) {
  this->mode = mode;
  server_sock = -1;
  if (this->mode != WIFI_SOCK_MODE_OFF) {
    switch (this->mode) {
      case WIFI_SOCK_MODE_STA_CL:
        // start_mdns_service();
        this->init_as_sta(ssid, pw);
        break;
      case WIFI_SOCK_MODE_AP_CL:
        this->init_as_ap(ssid, pw);
        break;
      default:
        log_w(TAG, "Unknown wifi mode: %d", mode);
        return;
    }
    // if (this->get_mode() == WIFI_SOCK_MODE_STA_CL ||
    //     this->get_mode() == WIFI_SOCK_MODE_AP_CL) {
    // start_task(wifi_tcp_client_task, "tcp_client", 4 * 1024, 5);
    // }
    // start_task(wifi_tcp_send_task, "tcp_send_server", 4 * 1024, 5);
  }
}

void TcpConnector::init_as_sta(const uint8_t* ssid, const uint8_t* pw) {
  // tcpip_adapter_init();
  // ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler_sta, NULL));

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_t* sta_netif = esp_netif_create_default_wifi_sta();
  assert(sta_netif);

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                             &wifi_event_handler_sta, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                             &wifi_event_handler_sta, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_GOT_IP6,
                                             &wifi_event_handler_sta, NULL));

  // ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  wifi_config_t wifi_config = {};

  // settings.get_wifi_ssid(wifi_config.sta.ssid);
  // settings.get_wifi_pw(wifi_config.sta.password);
  // FIXME: reals sting len here
  memcpy(wifi_config.sta.ssid, ssid, 32);
  memcpy(wifi_config.sta.password, pw, 32);
  wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
  wifi_config.sta.pmf_cfg.capable = true;
  wifi_config.sta.pmf_cfg.required = false;

  log_i(TAG, "STA, set WiFi configuration ssid: <%s>, pw: <%s>",
        wifi_config.sta.ssid, wifi_config.sta.password);

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  log_i(TAG, "ESP WiFi started in STA mode \n");
  //	uint32_t bits = IPV4_GOTIP_BIT | IPV6_GOTIP_BIT;
  // log_i(TAG, "Waiting for STA connection...");
  wifi_connected = true;
}

void TcpConnector::init_as_ap(const uint8_t* ssid, const uint8_t* pw) {
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_ap();

  // initialize the tcp stack
  // tcpip_adapter_init();
  // stop DHCP server
  // ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
  // assign a static IP to the network interface

  // tcpip_adapter_ip_info_t info = {};
  // memset(&info, 0, sizeof(info));
  // sprintf((char*)this->ipAddress, "192.168.1.1");
  // this->ipAddressLen = 11;
  // IP4_ADDR(&info.ip, 192, 168, 1, 1);
  // // ESP acts as router, so gw addr will be its own addr
  // IP4_ADDR(&info.gw, 192, 168, 1, 1);
  // IP4_ADDR(&info.netmask, 255, 255, 255, 0);
  // ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info));
  // // start the DHCP server
  // ESP_ERROR_CHECK(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP));
  // log_i(TAG, "DHCP server started \n");

  //	esp_log_level_set("wifi", ESP_LOG_NONE); // disable wifi driver logging
  // ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler_ap, NULL));
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  // ESP_ERROR_CHECK(esp_event_handler_instance_register(
  //     WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler_ap, NULL, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                             &wifi_event_handler_ap, NULL));

  // ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

  // configure the wifi connection and start the interface
  wifi_config_t ap_config = {};
  ap_config.ap = {};

  // FIXME: reals sting len here
  memcpy(ap_config.ap.ssid, ssid, strlen((const char*)ssid));
  ap_config.ap.ssid_len = strlen((const char*)ssid);
  memcpy(ap_config.ap.password, pw, strlen((const char*)pw));

  ap_config.ap.ssid_len = 0;
  ap_config.ap.channel = 0;
  ap_config.ap.authmode = WIFI_AUTH_WPA2_PSK;
  ap_config.ap.ssid_hidden = 0;
  ap_config.ap.max_connection = 1;
  ap_config.ap.beacon_interval = 100;

  log_i(TAG, "AP, set WiFi configuration ssid: <%s>, pw: <%s>",
        ap_config.ap.ssid, ap_config.ap.password);

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
  ESP_ERROR_CHECK(esp_wifi_start());
  log_i(TAG, "ESP WiFi started in AP mode \n");
}

void TcpConnector::wait_for_startup() {
  switch (this->mode) {
    case WIFI_SOCK_MODE_STA_CL: {
      uint32_t bits =
          tcp_event_group.wait_for_one(IPV4_GOTIP_BIT | IPV6_GOTIP_BIT);
      tcp_event_group.set(bits);
      printf("CONNECTED TO WIFI (STA)!");
      break;
    }
    case WIFI_SOCK_MODE_AP_CL: {
      tcp_event_group.wait_for(AP_STARTED_BIT);
      tcp_event_group.set(AP_STARTED_BIT);
      printf("CONNECTED TO WIFI (AP)!");
      break;
    }
    default:
      log_w(TAG, "Unknown wifi mode: %d", this->mode);
      break;
  }
}

bool TcpConnector::wait_for_client_connected() {
  // timeout after 10 sec. incase the client stays connected but reboots the
  // backend
  // xEventGroupWaitBits(tcp_event_group, CLIENT_CONNECTED_BIT, true, true,
  //                     pdMS_TO_TICKS(10000));
  if (tcp_event_group.wait_for(CLIENT_CONNECTED_BIT, 10000)) {
    tcp_event_group.set(CLIENT_CONNECTED_BIT);
    return true;
  }
  return false;
}

bool TcpConnector::connect_to_server(const uint8_t* host_ip,
                                     uint32_t host_port) {
  log_i(TAG, "connect to: %s:%d", host_ip, host_port);
  log_i(TAG, "connect_to_server: waiting for wifi connection");
  this->wait_for_startup();
  if (!this->wifi_connected) {
    log_i(TAG, "connect_to_server: waiting for peer");
    if (!this->wait_for_client_connected()) {
      return false;
    }
  }
  // log_i(TAG, "wifi_tcp_client_task: got ip");

  // uint8_t rx_buffer[TCP_IN_BUFFER_LEN];
  // int rx_buff_rec;
  char addr_str[128];

  int addr_family;
  int ip_protocol;
  // uint8_t host_ip[SETTING_IP_MAX_LEN];
  // memset(host_ip, 0x00, SETTING_IP_MAX_LEN);
  // settings.get_gsm_tcp_server_ip(host_ip);
#ifdef CONFIG_IPV4
  struct sockaddr_in dest_addr;
  dest_addr.sin_addr.s_addr = inet_addr((const char*)host_ip);
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(host_port);
  addr_family = AF_INET;
  ip_protocol = IPPROTO_IP;
  inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);
  // log_i(TAG, "set server ip: %s, port: %d", host_ip,
  //          settings.get_gsm_tcp_server_port());
#else  // IPV6
  struct sockaddr_in6 dest_addr;
  inet6_aton((const char*)host_ip, &dest_addr.sin6_addr);
  dest_addr.sin6_family = AF_INET6;
  dest_addr.sin6_port = htons(host_port);
  addr_family = AF_INET6;
  ip_protocol = IPPROTO_IPV6;
  inet6_ntoa_r(dest_addr.sin6_addr, addr_str, sizeof(addr_str) - 1);
#endif

  // while (1) {
  // log_i(TAG, "wifi_tcp_client_task: wait for client to connect.");
  // log_i(TAG, "wifi_tcp_client_task: connected to internet!");
  // update server address

  // memset(host_ip, 0x00, SETTING_IP_MAX_LEN);
  // settings.get_gsm_tcp_server_ip(host_ip);
  // dest_addr.sin_addr.s_addr = inet_addr((const char*)host_ip);
  // inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);
  // log_i(TAG, "set server ip: %s, port: %d", host_ip, host_port);

  server_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
  if (server_sock < 0) {
    log_e(TAG, "tcp_client: Unable to create socket: %s, ret %d",
          strerror(errno), server_sock);
    task_delay_ms(InternetReconnectTryPause);
  } else {
    log_i(TAG, "tcp_client: Socket created");
    int err =
        connect(server_sock, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
      log_w(TAG, "tcp_client: Socket unable to connect to server: %s",
            strerror(errno));
      // task_delay_ms(ServerReconnectTryPause);
    } else {
      tcp_connected = true;
      return true;
    }
  }
  tcp_connected = false;
  this->disconnect_from_server();
  // delay before reconnect, to prevent ddos
  // task_delay_ms(ServerReconnectTryPause);
  // }
  // end_task();
  return false;
}

bool TcpConnector::socket_is_open() {
  int error = 0;
  socklen_t len = sizeof(error);
  int retval = getsockopt(server_sock, SOL_SOCKET, SO_ERROR, &error, &len);
  if (retval != 0) {
    /* there was a problem getting the error code */
    log_e(TAG, "error getting socket error code(%d): %s", retval,
          strerror(retval));
    return false;
  }
  if (error == 104) {
    log_e(TAG, "Connection reset by peer, error code(%d)", error);
    return false;
  }

  if (error != 0 and error != 11) {  // 11: No more processes (always
    // comming...)
    /* socket has a non zero error status */
    log_e(TAG, "socket error: %d -> %s", error, strerror(error));
    return false;
  }
  return true;
}

void TcpConnector::disconnect_from_server() {
  // tcp_connected = false;
  if (server_sock != -1) {
    // log_e(TAG, "tcp_client: Shutting down socket and restarting...");
    shutdown(server_sock, 0);
    close(server_sock);
    server_sock = -1;
  }
}

int TcpConnector::read_bytes(uint8_t* rx_buffer, uint16_t read_len) {
  // blocking until received
  int len = recv(server_sock, rx_buffer, read_len, MSG_DONTWAIT);
  return (len);
}

int TcpConnector::write_bytes(uint8_t* bytes, uint16_t len) {
  if (this->is_connected() and tcp_connected) {
    int err = send(server_sock, bytes, len, 0);
    // printf("TcpConnector: send cnt: %d, err: %d, cmd: %x\n",
    //        bytes[USR_CMD_COUNT_BYTE_OFFSET], err,
    //        bytes[USR_CMD_CMD_BYTE_OFFSET]);
    if (err < 0) {
      printf("TcpConnector: Error occured during sending: errno %s, ret %d",
             strerror(errno), err);
      this->send_error_counter++;
      if (this->send_error_counter >= SEND_FAIL_MAX_FOR_DISCONNECT) {
        printf("TcpConnector: hit send fail max, disconnect!");
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