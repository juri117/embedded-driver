/**
 * @file neoPixel.h
 * @author
 * @brief
 * @version 0.1
 * @date 2020-10-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_PI3_NEOPIXEL_H
#define DRIVER_DRIVER_PI3_NEOPIXEL_H

#include <driver/gpio.h>

#include "../error.hpp"
#include "log.hpp"

class NeoPixel {
 private:
 public:
  NeoPixel();
  void init(gpio_num_t gpio, uint16_t neo_count);
  void set_color(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
  void set_brightness(uint16_t index, float brightness);
};

#endif /* DRIVER_DRIVER_PI3_NEOPIXEL_H */
