

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

void init_onboard_led() { pinMode(LED_GPIO, OUTPUT); }

void init_interrupt_out(int pin_num) { pinMode(pin_num, OUTPUT); }

void set_interrupt_out(int pin_num, bool on) { digitalWrite(pin_num, on); }

void init_onboard_button() { pinMode(BUTTON_GPIO, INPUT); }

void set_led(bool on) { digitalWrite(LED_GPIO, on); }

bool button_is_pressed() { return digitalRead(BUTTON_GPIO); }

void reboot() {
  // std::exit(42);
  system("sudo reboot now");
}

void sleep() { system("sudo shutdown now"); }

uint64_t get_efuse_mac() {
  uint8_t mac_buff[6];
  get_efuse_mac(mac_buff);
  return mac_buff[0] + ((uint64_t)mac_buff[1] << 8) +
         ((uint64_t)mac_buff[2] << 16) + ((uint64_t)mac_buff[3] << 24) +
         ((uint64_t)mac_buff[4] << 32) + ((uint64_t)mac_buff[5] << 40);
}

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

Adc::Adc() {}

void Adc::init(adc_input_t channel) {}

uint32_t Adc::read_raw() { return 0; }

uint32_t Adc::calc_voltage(uint32_t adc_reading) { return 0; }

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

uint16_t get_cpu_load_this_process() {
  std::string res = exec("ps -C \"main\" -o \%cpu,\%mem");
  float load, mem;
  int n = sscanf(res.c_str(), "%*s %*s %f %f", &load, &mem);
  return (uint8_t)(load * 10000);
}

uint16_t get_cpu_temp() {
  uint16_t millideg;
  char buff[1024];
  std::ifstream file;
  file.open("/sys/class/thermal/thermal_zone0/temp");
  if (file.bad()) {
    return ERROR_FAIL;
  }
  file.read(buff, sizeof(buff) - 1);
  sscanf(buff, "%d", &millideg);
  file.close();
  return millideg;
}

#endif