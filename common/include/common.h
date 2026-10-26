#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define set_bit(reg, bit) (reg |= (1 << (bit)))
#define clear_bit(reg, bit) (reg &= ~(1 << (bit)))
#define test_bit(reg, bit) (reg & (1 << (bit)))

struct CAN_Message {
  uint16_t ID;
  uint8_t data_length;
  uint8_t data[8];
};

#endif // COMMON_H
