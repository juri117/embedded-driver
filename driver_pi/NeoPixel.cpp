
#include "NeoPixel.hpp"

#if BUILD_TARGET == TARGET_PI3

// static const char* TAG = "neoPixel";

NeoPixel::NeoPixel() {}
void NeoPixel::init(gpio_num_t gpio, uint16_t neo_count) {}
void NeoPixel::set_color(uint16_t index, uint8_t r, uint8_t g, uint8_t b) {}
void NeoPixel::set_brightness(uint16_t index, float brightness) {}

#endif