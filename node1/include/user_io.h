// This module is responsible for translating User IO input into usable data.

#ifndef USER_IO_H_
#define USER_IO_H_

#include "serialize.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  float x;
  float y;
} vec2_t;

extern vec2_t joy_neutral;

typedef enum {
  JOY_LEFT,
  JOY_RIGHT,
  JOY_UP,
  JOY_DOWN,
  JOY_NEUTRAL,
} JoyDir;

void joy_calibrate(void);

vec2_t joy_read_vec(void);

JoyDir joy_read_dir(void);

vec2_t pad_read_vec(void);

Buttons read_buttons(void);

void led_init(void);

// `led_n` is between 0 and 5.
// `value` is 0 or 1.
void set_led(uint8_t led_n, uint8_t value);

// `led_n` is between 0 and 5.
// `brightness` is a value between 0 (min) and 255 (max).
void set_led_pwm(uint8_t led_n, uint8_t brightness);

#endif // USER_IO_H_
