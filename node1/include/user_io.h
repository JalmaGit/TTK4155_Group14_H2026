// This module is responsible for translating User IO input into usable data.

#ifndef USER_IO_H_
#define USER_IO_H_

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

// user-io avr buttons
typedef struct __attribute__((packed)) {
  union {
    uint8_t right;
    struct {
      uint8_t R1 : 1;
      uint8_t R2 : 1;
      uint8_t R3 : 1;
      uint8_t R4 : 1;
      uint8_t R5 : 1;
      uint8_t R6 : 1;
    };
  };
  union {
    uint8_t left;
    struct {
      uint8_t L1 : 1;
      uint8_t L2 : 1;
      uint8_t L3 : 1;
      uint8_t L4 : 1;
      uint8_t L5 : 1;
      uint8_t L6 : 1;
      uint8_t L7 : 1;
    };
  };
  union {
    uint8_t nav;
    struct {
      uint8_t NB : 1;
      uint8_t NR : 1;
      uint8_t ND : 1;
      uint8_t NL : 1;
      uint8_t NU : 1;
    };
  };
} Buttons;

Buttons read_buttons(void);

#endif // USER_IO_H_
