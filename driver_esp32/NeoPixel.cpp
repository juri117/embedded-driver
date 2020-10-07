/**
 * @file neoPixel.cpp
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

#include "NeoPixel.hpp"

#if BUILD_TARGET == TARGET_ESP32

// static const char* TAG = "neoPixel";

NeoPixel::NeoPixel() {}
void NeoPixel::init(gpio_num_t gpio, uint16_t neo_count) {}
void NeoPixel::set_color(uint16_t index, uint8_t r, uint8_t g, uint8_t b) {}
void NeoPixel::set_brightness(uint16_t index, float brightness) {}

#endif