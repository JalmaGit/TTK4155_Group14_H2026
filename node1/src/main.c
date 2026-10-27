#include "adc.h"
#include "board.h"
#include "common.h"
#include "display/oled.h"
#include "mcp2515_can.h"
#include "spi.h"
#include "uart.h"
#include "ui/menu_items.h"
#include "user_io.h"
#include "xmem.h"
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

int main() {
  uart_init(MY_UBRR);
  printf("UART Initialized...\r\n");
  _delay_ms(500);
  printf("Waited 500ms... \r\n");
  /*
  spi_master_init();

  _delay_ms(500);

  xmem_init();
  adc_init();
  led_init();
  joy_calibrate(); // Joystick needs to be in neutral position

  _delay_ms(1000);

  oled_init();
  printf("OLED Initialized...");

  _delay_ms(100);

  menu_main();
  */

  uint8_t init_result = can_init();

  printf("CAN init result: %i \r\n", init_result);

  struct CAN_Message can_received_message;

  struct CAN_Message can_transmit_message;
  can_transmit_message.ID = 0x0001;
  can_transmit_message.data_length = 1;

  uint8_t iter = 200;

  while (1) {
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
}
