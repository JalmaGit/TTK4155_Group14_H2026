#ifndef CAN_H
#define CAN_H

#include <stdint.h>

struct CAN_Message {
  uint16_t ID;
  uint8_t data_length;
  uint8_t data[8];
};

struct CAN_Debug_Message {
  uint8_t read_status;
  uint8_t can_status;
  uint8_t can_interrupt_flag;
  uint8_t error_flag;
  uint8_t transmitter_error_flag;
  uint8_t receiver_error_flag;
};

#endif // CAN_H
