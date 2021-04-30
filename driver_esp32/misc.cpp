

#include "misc.hpp"

#if BUILD_TARGET == TARGET_ESP32

void init_gpio_isr_service() { gpio_install_isr_service(0); }
error_t register_gpio_interrupt(gpio_num_t gpio_num, gpio_isr_t isr_handler,
                                void* args) {
  return (error_t)gpio_isr_handler_add(gpio_num, isr_handler, args);
}

void init_nvs() {
  error_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
    // NVS partition was truncated and needs to be erased
    // log_e(TAG, "NVS could not be initialised, try to erase it first...");
    ESP_ERROR_CHECK(nvs_flash_erase());
    // Retry nvs_flash_init
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
}

void init_onboard_led() { init_gpio_out(LED_GPIO); }

void set_led(bool on) { set_gpio_out(LED_GPIO, on); }

void init_gpio_out(gpio_num_t pin_num) {
  gpio_pad_select_gpio(pin_num);  // Set pin as GPIO
  gpio_set_direction(pin_num,
                     GPIO_MODE_OUTPUT);  // Set as Output with open drain
  // gpio_pullup_en(pin_num);
  gpio_pulldown_en(pin_num);
}

void set_gpio_out(gpio_num_t pin_num, bool on) { gpio_set_level(pin_num, on); }

void init_onboard_button() { init_gpio_in(BUTTON_GPIO); }

bool button_is_pressed() { return (not get_gpio_in(BUTTON_GPIO)); }

void init_gpio_in(int pin_num) {
  gpio_config_t btn_config = {};
  btn_config.mode = GPIO_MODE_INPUT;                // Set as Input
  btn_config.pin_bit_mask = (1 << pin_num);         // Bitmask
  btn_config.pull_up_en = GPIO_PULLUP_ENABLE;       // Disable pullup
  btn_config.pull_down_en = GPIO_PULLDOWN_DISABLE;  // Enable pulldown
  gpio_config(&btn_config);
}

bool get_gpio_in(int pin_num) { return gpio_get_level(BUTTON_GPIO); }

void reboot() { esp_restart(); }

void sleep() { esp_deep_sleep_start(); }

uint64_t get_efuse_mac() {
  uint64_t _chipmacid = 0LL;
  esp_efuse_mac_get_default((uint8_t*)(&_chipmacid));
  return _chipmacid;
}

error_t get_efuse_mac(uint8_t* mac) { return esp_efuse_mac_get_default(mac); }

Adc::Adc() : adc_chars(), adc_chan() {}

void Adc::init(adc_input_t channel) {
  this->adc_chan = channel;
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten((adc1_channel_t)this->adc_chan, ADC_ATTEN_DB_11);
  this->adc_chars = (esp_adc_cal_characteristics_t*)calloc(
      1, sizeof(esp_adc_cal_characteristics_t));
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100,
                           adc_chars);
}

uint32_t Adc::read_raw() {
  return adc1_get_raw((adc1_channel_t)this->adc_chan);
}

uint32_t Adc::calc_voltage(uint32_t adc_reading) {
  return esp_adc_cal_raw_to_voltage(adc_reading, this->adc_chars);
}

uint16_t get_cpu_load_total() { return 0; }
uint16_t get_cpu_load_this_process() { return 0; }
int16_t get_cpu_temp() { return 0; }
uint32_t get_system_error_code() { return0; }

#endif