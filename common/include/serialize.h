#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <stdint.h>

// Serializes a float to an uint8 between 0 and 255 (lossy).
// Clamps values out of range to 0 or 255.
// NaN is set to 0.
uint8_t float_to_byte(float value, float range_start, float range_end);

// Deserializes a byte made with `float_to_byte` as a float.
float byte_to_float(uint8_t value, float range_start, float range_end);

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

#endif // SERIALIZE_H
