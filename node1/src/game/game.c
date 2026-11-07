#include "game/game.h"
#include "can/mcp2515_high_level.h"
#include "game/game_ui.h"
#include "game/tick.h"
#include "serialize.h"
#include "user_io.h"
#include <stdint.h>
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

#define SECONDS(s) ((uint32_t)(s) * TICK_HZ)

void game_loop() {
  tick_init();

  int score = 0;
  uint32_t now = 0;

  int next_update_score = SECONDS(1);

  while (1) {
    uint8_t missed = tick_wait();
    now += 1 + missed;

    bool update_screen = false;

    send_joystick_data();

    if (now >= next_update_score) {
      next_update_score += SECONDS(1);
      score += 1;
      update_screen = true;
    }

    if (update_screen) {
      game_draw(score);
    }
  }

  tick_stop();
}

void game_start() {
  printf("starting game...");

  game_loop();

  printf("game finished...");
}
