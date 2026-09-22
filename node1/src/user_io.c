#include "user_io.h"

#define JOY_X_CH 1
#define JOY_Y_CH 0
#define SLIDER_X_CH 3
#define SLIDER_Y_CH 2

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
