#include "can.h"
#include "can/mcp2515_high_level.h"
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

struct CAN_Message can_received_message;
struct CAN_Message can_transmit_message;
uint8_t iter = 0;

void init_can_bus_test(void) {
  uint8_t init_result = can_init();
  enable_loop_back_mode();

  printf("CAN init result: %i \r\n", init_result);

  can_transmit_message.ID = 0x0001;
  can_transmit_message.data_length = 1;
}

void can_bus_test(void) {
  can_transmit_message.data[0] = iter;
  can_transmit(&can_transmit_message);
  _delay_ms(100);
  can_received_message = can_wait_receive();
  _delay_ms(500);
  printf("Transmited Data: \r\n");
  printf("Message ID: %" PRIu16 " \r\n", can_transmit_message.ID);
  printf("Message Length: %i \r\n", can_transmit_message.data_length);
  printf("Message Data: %i \r\n", can_transmit_message.data[0]);

  printf("\r\n");

  printf("Received Data: \r\n");
  printf("Message ID: %" PRIu16 " \r\n", can_received_message.ID);
  printf("Message Length: %i \r\n", can_received_message.data_length);
  printf("Message Data: %i \r\n", can_received_message.data[0]);

  printf("_________________________________\r\n");

  iter++;
}
