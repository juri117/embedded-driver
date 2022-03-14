

#include "misc.hpp"

#if BUILD_TARGET == TARGET_PI3

static const char *TAG = "misc";

void init_gpio_isr_service() {
  if (wiringPiSetup() < 0) {
    log_e(TAG, "Unable to setup wiringPi: %s", strerror(errno));
  }
}
error_t register_gpio_interrupt(int gpio_num, void (*function)(), void *args) {
  // pinMode(gpio_num, OUTPUT);
  pullUpDnControl(gpio_num, PUD_DOWN);
  if (wiringPiISR(gpio_num, INT_EDGE_RISING, function) < 0) {
    log_e(TAG, "Unable to setup wiringPiISR: %s", strerror(errno));
    return ERROR_FAIL;
  }
  return ERROR_OK;
}

void init_nvs() {}

void init_onboard_led() {
  // pinMode(LED_GPIO, OUTPUT);
  init_gpio_out(LED_GPIO);
}

void set_led(bool on) { set_gpio_out(LED_GPIO, on); }

void init_gpio_out(int pin_num) { pinMode(pin_num, OUTPUT); }

void set_gpio_out(int pin_num, bool on) { digitalWrite(pin_num, on); }

void init_onboard_button() { init_gpio_in(BUTTON_GPIO); }

bool button_is_pressed() { return get_gpio_in(BUTTON_GPIO); }

void init_gpio_in(int pin_num) {
  pinMode(pin_num, INPUT);
  pullUpDnControl(pin_num, PUD_DOWN);
}

bool get_gpio_in(int pin_num) { return digitalRead(pin_num); }

void reboot() {
  // std::exit(42);
  system("sudo reboot now");
}

void sleep() { system("sudo shutdown now"); }

/**
 * @brief Get mac address of wlan0
 *
 * @return uint64_t mac as one number
 */
uint64_t get_efuse_mac() {
  uint8_t mac_buff[6];
  get_efuse_mac(mac_buff);
  return mac_buff[0] + ((uint64_t)mac_buff[1] << 8) +
         ((uint64_t)mac_buff[2] << 16) + ((uint64_t)mac_buff[3] << 24) +
         ((uint64_t)mac_buff[4] << 32) + ((uint64_t)mac_buff[5] << 40);
}

/**
 * @brief Get mac address of wlan0
 *
 * @param mac[6] it will be written to this buffer
 * @return error_t
 */
error_t get_efuse_mac(uint8_t *mac) {
  struct ifreq s;
  int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

  strcpy(s.ifr_name, "wlan0");
  if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) {
    memcpy(mac, s.ifr_addr.sa_data, 6);
    return ERROR_OK;
  }
  return ERROR_FAIL;
}

// TODO
Adc::Adc() {}
void Adc::init(adc_input_t channel) {}
uint32_t Adc::read_raw() { return 0; }
uint32_t Adc::calc_voltage(uint32_t adc_reading) { return 0; }

/**
 * @brief Get the total cpu load
 *
 * @return uint16_t cpu load in percent * 100
 */
uint16_t get_cpu_load_total() {
  std::ifstream file;
  char buff[1024];
  float load;
  file.open("/proc/loadavg");
  if (file.bad()) {
    return ERROR_FAIL;
  }
  file.read(buff, sizeof(buff) - 1);
  sscanf(buff, "%f", &load);
  file.close();
  return (uint16_t)(load * 10000);
}

/**
 * @brief Get the cpu load of this process
 *
 * @return uint16_t cpu load in percent * 100
 */
uint16_t get_cpu_load_this_process() {
  // std::string res = exec("ps -C \"main\" -o \%cpu,\%mem");
  exec_return_t res = exec("ps -C \"main\" -o \%cpu,\%mem");
  float load, mem;
  int n = sscanf(res.result.c_str(), "%*s %*s %f %f", &load, &mem);
  // ToDo: check n if it worked
  return (uint16_t)(load * 100);
}

/**
 * @brief Get the cpu temperature
 *
 * @return int16_t temperature in C * 100
 */
int16_t get_cpu_temp() {
  int32_t millideg;
  char buff[1024];
  std::ifstream file;
  file.open("/sys/class/thermal/thermal_zone0/temp");
  if (file.bad()) {
    return ERROR_FAIL;
  }
  file.read(buff, sizeof(buff) - 1);
  sscanf(buff, "%d", &millideg);
  file.close();
  return millideg / 10;
}

/**
 * @brief Get the system error code by calling vcgencmd get_throttled
 * https://github.com/raspberrypi/documentation/blob/master/raspbian/applications/vcgencmd.md
 *
 * @return uint32_t error code
 */
uint32_t get_system_error_code() {
  // std::string res = exec("vcgencmd get_throttled");
  exec_return_t res = exec("vcgencmd get_throttled");
  uint32_t code;
  int n = sscanf(res.result.c_str(), "throttled=%x\n", &code);
  // ToDo: check n if it worked
  return code;
}

#endif