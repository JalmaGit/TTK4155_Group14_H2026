#ifndef USER_IO_H_
#define USER_IO_H_

// This module is responsible for translating User IO input into usable data.

#include <stdbool.h>

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

#endif // USER_IO_H_
