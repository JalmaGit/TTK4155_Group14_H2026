#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <stdint.h>

// Serializes a float to an uint8 between 0 and 255 (lossy).
// Clamps values out of range to 0 or 255.
// NaN is set to 0.
uint8_t float_to_byte(float value, float range_start, float range_end);

// Deserializes a byte made with `float_to_byte` as a float.
float byte_to_float(uint8_t value, float range_start, float range_end);

#endif // SERIALIZE_H
