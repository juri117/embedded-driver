/**
 * @file neoPixel.h
 * @author  Juri Bieler, Michael Reno (michaelreno19@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-10-06
 *
 * @copyright Copyright (c) 2020
 * with some codes from https://github.com/FozzTexx/ws2812-demo
 * Created 19 Nov 2016 by Chris Osborn <fozztexx@fozztexx.com>
 * http://insentricity.com
 * 
 */

#ifndef DRIVER_DRIVER_PI3_NEOPIXEL_H
#define DRIVER_DRIVER_PI3_NEOPIXEL_H

#include <driver/gpio.h>

#include "../error.hpp"
#include "log.hpp"
#include <stdint.h>

typedef union {
  struct __attribute__ ((packed)) {
    uint8_t r, g, b;
  };
  uint32_t num;
} rgbVal;

class NeoPixel {
 private:
 public:
  NeoPixel();
  void init(gpio_num_t gpio, uint16_t neo_count);
  void set_color(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
  void set_brightness(uint16_t index, float brightness);
};

inline rgbVal makeRGBVal(uint8_t r, uint8_t g, uint8_t b)
{
  rgbVal v;


  v.r = r;
  v.g = g;
  v.b = b;
  return v;
}

#endif /* DRIVER_DRIVER_PI3_NEOPIXEL_H */
