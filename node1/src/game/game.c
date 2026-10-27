#include "can/mcp2515_high_level.h"
#include "serialize.h"
#include "user_io.h"
#include <stdio.h>
#include <util/delay.h>

void send_joystick_data() {
  vec2_t joy_vec = joy_read_vec();

  // TODO: Move joystick data message to common.
  struct CAN_Message msg = {0x7,
                            2,
                            {
                                float_to_byte(joy_vec.x, -1, 1),
                                float_to_byte(joy_vec.y, -1, 1),
                            }};

  can_transmit(&msg);
}

void game_loop() {
  while (1) {
    send_joystick_data();

    _delay_ms(20);
  }
}

void game_start() {
  printf("starting game...");

  game_loop();

  printf("game finished...");
}
