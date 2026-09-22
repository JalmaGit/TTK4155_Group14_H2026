#include "user_io.h"
#include "adc.h"
#include <stdio.h>
#include <util/delay.h>

#define JOY_X_CH 1
#define JOY_Y_CH 0
#define SLIDER_X_CH 3
#define SLIDER_Y_CH 2

// Hardcoded values for the joystick's min and max values, 
// based on the ADC readings. These values may need to be 
// adjusted based on the specific joystick being used.
static float joy_x_max = 248.0;
static float joy_y_max = 244.0;
static float joy_x_min = 65.0;
static float joy_y_min = 71.0;

vec2_t joy_neutral = {0.0, 0.0};

void joy_calibrate(void) {
  joy_neutral.x = (float) adc_read(JOY_X_CH);
  joy_neutral.y = (float) adc_read(JOY_Y_CH);
}

vec2_t joy_read_vec(void) {
  // Neutral is 0, max is 1, min is -1
  vec2_t joy_pos = joy_read_vec_raw();
  normalize(&joy_pos);
  return joy_pos;
}

vec2_t joy_read_vec_raw(void) {
  vec2_t joy_pos = {0.0, 0.0};
  joy_pos.x = (float) adc_read(JOY_X_CH);
  joy_pos.y = (float) adc_read(JOY_Y_CH);
  return joy_pos;
}

void normalize(vec2_t *vec) {
  // Normalizes the ranges -1 to 0 and 0 to 1 individually 
  // for x and y, based on the calibrated neutral position 
  // and the min/max values.
  if (vec->x == joy_neutral.x) {
    vec->x = 0.0;
  } else if (vec->x > joy_neutral.x) {
    vec->x = (vec->x - joy_neutral.x) / (joy_x_max - joy_neutral.x);
  } else {
    vec->x = (vec->x - joy_neutral.x) / (joy_neutral.x - joy_x_min);
  }

  if (vec->y == joy_neutral.y) {
    vec->y = 0.0;
  } else if (vec->y > joy_neutral.y) {
    vec->y = (vec->y - joy_neutral.y) / (joy_y_max - joy_neutral.y);
  } else {
    vec->y = (vec->y - joy_neutral.y) / (joy_neutral.y - joy_y_min);
  }
}

JoyDir joy_read_dir(void) {
  // Returns the main direction of the joystick based on 
  // the normalized x and y values.

  vec2_t joy_pos = joy_read_vec();
  if (joy_pos.x > 0.8) {
    return JOY_RIGHT;
  } else if (joy_pos.x < -0.8) {
    return JOY_LEFT;
  } else if (joy_pos.y > 0.8) {
    return JOY_UP;
  } else if (joy_pos.y < -0.8) {
    return JOY_DOWN;
  } else {
    return JOY_NEUTRAL;
  }
}


