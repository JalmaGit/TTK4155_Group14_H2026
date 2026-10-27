#ifndef CAN_CONFIG_H
#define CAN_CONFIG_H

#include <stdint.h>

#define CAN_LOOP_BACK_MODE 0b01000000
#define CAN_NORMAL_MODE 0b00000000

void bit_timings_configure();
void interrupt_configure();
void receive_configure();
void mode_configure(uint8_t MODE);

#endif // CAN_CONFIG_H
