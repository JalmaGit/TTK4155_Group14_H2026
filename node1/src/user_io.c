#include "user_io.h"

void joy_calibrate(void) {
  // TODO
}

vec2_t joy_read_vec(void) {
  // TODO: Read raw ADC data, and offset/scale.
  vec2_t joy_pos = {0.0, 0.0};
  return joy_pos;
}

JoyDir joy_read_dir(void) {
  JoyDir joy_dir = JOY_NEUTRAL;
  return joy_dir;
}
