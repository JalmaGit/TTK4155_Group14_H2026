#include "game/game.h"
#include "can.h"
#include "can/mcp2515_high_level.h"
#include "game/game_ui.h"
#include "game/tick.h"
#include "msgs.h"
#include "serialize.h"
#include "user_io.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

void send_joystick_data() {
  vec2_t joy_vec = joy_read_vec();
  Buttons buttons = read_buttons();

  // TODO: Move joystick data message to common.
  struct CAN_Message msg = {MSG_USER_IO_ID,
                            5,
                            {
                                float_to_byte(joy_vec.x, -1, 1),
                                float_to_byte(joy_vec.y, -1, 1),
                                buttons.right,
                                buttons.left,
                                buttons.nav,
                            }};

  can_transmit(&msg);
}

#define SECONDS(s) ((uint32_t)(s) * TICK_HZ)

void set_lights(int lives) {
  set_led(0, lives >= 1);
  set_led(1, lives >= 2);
  set_led(2, lives >= 3);
}

void game_loop() {
  tick_init();

  int score = 0;
  uint32_t now = 0;
  int lives = 3;

  int next_update_score = SECONDS(1);
  int next_restart = SECONDS(0);

  bool is_ir_blocked = false;
  int ir_blocked_strikes = 0;
  bool is_respawning = false;

  set_lights(lives);

  game_draw(score, is_respawning);

  while (1) {
    uint8_t missed = tick_wait();
    now += 1 + missed;

    bool update_screen = false;

    struct CAN_Message msg_incoming = can_wait_receive();

    if (msg_incoming.ID != 0xFFFF) {
      switch (msg_incoming.ID) {
      case MSG_IR_BLOCKED_ID:
        is_ir_blocked = msg_incoming.data[0];
        break;
      }
    }

    if (now <= next_restart) {
      // wait
      continue;
    }

    is_respawning = false;

    send_joystick_data();

    if (now >= next_update_score) {
      next_update_score += SECONDS(1);
      score += 1;
      update_screen = true;
    }

    if (is_ir_blocked) {
      ir_blocked_strikes += 1;
    } else {
      ir_blocked_strikes = 0;
    }

    if (ir_blocked_strikes >= 3) {
      lives -= 1;
      set_lights(lives);
      is_respawning = true;
      update_screen = true;
      next_restart = now + SECONDS(2);
    }

    if (lives <= 0) {
      break;
    }

    if (update_screen) {
      game_draw(score, is_respawning);
    }
  }

  tick_stop();
}

void game_start() {
  printf("starting game...");

  game_loop();

  printf("game finished...");
}
