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

#ifndef DRIVER_DRIVER_PI3_MISC_H
#define DRIVER_DRIVER_PI3_MISC_H

#include <errno.h>
#include <linux/if.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <wiringPi.h>

#include <functional>

#include "../error.hpp"
#include "log.hpp"

typedef int adc_input_t;

#define ON 1
#define OFF 0

#define LED_GPIO 24     // just a suggestion
#define BUTTON_GPIO 23  // just a suggestion

void init_gpio_isr_service();
error_t register_gpio_interrupt(int gpio_num, void (*function)(), void* args);
void init_nvs();

void init_onboard_led();
void init_onboard_button();

void set_led(bool on);
bool button_is_pressed();

void init_interrupt_out(int pin_num);
void set_interrupt_out(int pin_num, bool on);

void reboot();

void sleep();

uint64_t get_efuse_mac();
error_t get_efuse_mac(uint8_t* mac);

class Adc {
 private:
 public:
  Adc();
  void init(adc_input_t channel);
  uint32_t read_raw();
  uint32_t calc_voltage(uint32_t adc_reading);
};

#endif /* DRIVER_DRIVER_PI3_MISC_H */
