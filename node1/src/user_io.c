#include "user_io.h"
#include "adc.h"
#include "spi.h"
#include <stdbool.h>
#include <util/delay.h>

#define JOY_X_CH 1
#define JOY_Y_CH 0
#define PAD_X_CH 3
#define PAD_Y_CH 2

// TODO: Use own file for CS and pin configs.
#define IO_CS 0

// Hardcoded values for the joystick's min and max values,
// based on the ADC readings. These values may need to be
// adjusted based on the specific joystick being used.
static float joy_x_max = 248.0;
static float joy_y_max = 244.0;
static float joy_x_min = 65.0;
static float joy_y_min = 71.0;

static float slider_x_max = 255.0;
static float slider_y_max = 255.0;
static float slider_x_min = 1.0;
static float slider_y_min = 1.0;

vec2_t joy_neutral = {0.0, 0.0};

void joy_calibrate(void) {
  joy_neutral.x = (float)adc_read(JOY_X_CH);
  joy_neutral.y = (float)adc_read(JOY_Y_CH);
}

vec2_t joy_read_vec_raw(void) {
  vec2_t joy_pos = {0.0, 0.0};
  joy_pos.x = (float)adc_read(JOY_X_CH);
  joy_pos.y = (float)adc_read(JOY_Y_CH);
  return joy_pos;
}

void normalize(vec2_t *vec, bool is_joystick) {
  // Normalizes the ranges -1 to 0 and 0 to 1 individually
  // for x and y, based on the calibrated neutral position
  // and the min/max values.

  // To generalize between pad and joystick, this value sets up
  // the min and max values for each axis based on the input type
  float x_max;
  float y_max;
  float x_min;
  float y_min;
  float x_neutral;
  float y_neutral;

  if (is_joystick) {
    x_max = joy_x_max;
    y_max = joy_y_max;
    x_min = joy_x_min;
    y_min = joy_y_min;
    x_neutral = joy_neutral.x;
    y_neutral = joy_neutral.y;
  } else {
    x_max = slider_x_max;
    y_max = slider_y_max;
    x_min = slider_x_min;
    y_min = slider_y_min;
    x_neutral = (slider_x_max + slider_x_min) / 2.0;
    y_neutral = (slider_y_max + slider_y_min) / 2.0;
  }

  if (vec->x == x_neutral) {
    vec->x = 0.0;
  } else if (vec->x > x_neutral) {
    vec->x = (vec->x - x_neutral) / (x_max - x_neutral);
  } else {
    vec->x = (vec->x - x_neutral) / (x_neutral - x_min);
  }

  if (vec->y == y_neutral) {
    vec->y = 0.0;
  } else if (vec->y > y_neutral) {
    vec->y = (vec->y - y_neutral) / (y_max - y_neutral);
  } else {
    vec->y = (vec->y - y_neutral) / (y_neutral - y_min);
  }
}

vec2_t joy_read_vec(void) {
  // Neutral is 0, max is 1, min is -1
  vec2_t joy_pos = joy_read_vec_raw();
  normalize(&joy_pos, true);
  return joy_pos;
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

vec2_t pad_read_vec_raw(void) {
  vec2_t pad_pos = {0.0, 0.0};
  pad_pos.x = (float)adc_read(PAD_X_CH);
  pad_pos.y = (float)adc_read(PAD_Y_CH);
  return pad_pos;
}

vec2_t pad_read_vec(void) {
  // Neutral is 0, max is 1, min is -1
  vec2_t pad_pos = pad_read_vec_raw();
  normalize(&pad_pos, false);
  return pad_pos;
}

// IO-Board AVR

Buttons read_buttons(void) {
  select_device(IO_CS);
  spi_master_transceive(0x04);
  _delay_us(40);
  uint8_t right = spi_master_transceive(0x00);
  _delay_us(2);
  uint8_t left = spi_master_transceive(0x00);
  _delay_us(2);
  uint8_t nav = spi_master_transceive(0x00);
  deselect_device(IO_CS);

  Buttons buttons;
  buttons.right = right;
  buttons.left = left;
  buttons.nav = nav;
  return buttons;
}

void led_init(void) {
  for (int i = 0; i <= 5; i++) {
    set_led(i, 0);
  }
}

// Note, the CS must be low during *all* transactions within one command.
void io_write_2_data(uint8_t cmd, uint8_t a, uint8_t b) {
  select_device(IO_CS);
  spi_master_transceive(cmd);
  _delay_us(40);
  spi_master_transceive(a);
  _delay_us(2);
  spi_master_transceive(b);
  deselect_device(IO_CS);
}

void set_led(uint8_t led_n, uint8_t value) {
  io_write_2_data(0x05, led_n, value);
}

void set_led_pwm(uint8_t led_n, uint8_t brightness) {
  io_write_2_data(0x06, led_n, brightness);
}
