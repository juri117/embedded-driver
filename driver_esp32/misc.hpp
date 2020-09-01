/**
 * @file misc.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_ESP32_MISC_H
#define DRIVER_DRIVER_ESP32_MISC_H

#include <driver/adc.h>
#include <driver/gpio.h>
#include <driver/periph_ctrl.h>
#include <driver/timer.h>
#include <driver/uart.h>
#include <esp_adc_cal.h>
#include <esp_sleep.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <soc/timer_group_struct.h>
#include <stdio.h>

#include "../error.hpp"

typedef adc1_channel_t adc_input_t;

#define ON 1
#define OFF 0

#define LED_GPIO GPIO_NUM_2     // default led gpio on devboard
#define BUTTON_GPIO GPIO_NUM_0  // default button gpio on devboard

void init_gpio_isr_service();
error_t register_gpio_interrupt(gpio_num_t gpio_num, gpio_isr_t isr_handler,
                                void* args);
void init_nvs();

void init_onboard_led();
void init_onboard_button();

void set_led(bool on);
bool button_is_pressed();

void init_interrupt_out(gpio_num_t pin_num);
void set_interrupt_out(gpio_num_t pin_num, bool on);

void reboot();

void sleep();

uint64_t get_efuse_mac();
error_t get_efuse_mac(uint8_t* mac);

class Adc {
 private:
  esp_adc_cal_characteristics_t* adc_chars;
  adc_input_t adc_chan;

 public:
  Adc();
  void init(adc_input_t channel);
  uint32_t read_raw();
  uint32_t calc_voltage(uint32_t adc_reading);
};

#endif /* DRIVER_DRIVER_ESP32_MISC_H */
