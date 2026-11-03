#include "serialize.h"
#include <stdint.h>

uint8_t float_to_byte(float value, float range_start, float range_end) {
  float width = range_end - range_start;
  float new_value = (value - range_start) / width * 255.0f;

  if (!(new_value > 0.0f)) {
    new_value = 0;
  } else if (new_value > 255.0f) {
    new_value = 255;
  }

  // Rounds to nearest
  return (uint8_t)(new_value + 0.5f);
}

float byte_to_float(uint8_t value, float range_start, float range_end) {
  float width = range_end - range_start;
  return value / 255.0f * width + range_start;
}
