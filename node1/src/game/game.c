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

void send_game_data(bool is_playing) {
  struct CAN_Message msg = {MSG_GAME_DATA_ID, 1, {is_playing}};
  can_transmit(&msg);
}

#define SECONDS(s) ((uint32_t)(s) * TICK_HZ)

void set_lights(int lives) {
  set_led(0, lives >= 1);
  set_led(1, lives >= 2);
  set_led(2, lives >= 3);
}

void block_until_continue(void) {
  bool continue_button_pressed = false;

  while (!continue_button_pressed) {
    Buttons buttons = read_buttons();

    continue_button_pressed = buttons.R6;
  }
}

#define CAN_NO_MESSAGE 0xFFFF

static void can_drain(void) {
  // Empties stale data
  for (uint8_t i = 0; i < 8; i++) {
    if (can_wait_receive().ID == CAN_NO_MESSAGE)
      break;
  }
}

void check_no_ball(void) {
  can_drain();
  bool ir_message_received = false;
  bool is_ir_blocked = false;

  while (!ir_message_received) {
    struct CAN_Message msg_incoming = can_wait_receive();

    if (msg_incoming.ID == MSG_IR_BLOCKED_ID) {
      is_ir_blocked = msg_incoming.data[0];
      ir_message_received = true;
    }
  }

  if (!is_ir_blocked) {
    return;
  }

  game_draw_ball_warning();

  while (is_ir_blocked) {
    struct CAN_Message msg_incoming = can_wait_receive();

    if (msg_incoming.ID == MSG_IR_BLOCKED_ID) {
      is_ir_blocked = msg_incoming.data[0];
    }
  }

  game_draw_press_continue();
  block_until_continue();
}

void game_round(int *score) {
  send_game_data(true);
  check_no_ball();
  game_draw(*score, false);

  tick_init();

  uint32_t now = 0;
  uint32_t next_update_score = SECONDS(1);

  int is_ir_blocked = false;
  int is_ir_blocked_strikes = 0;
  bool redraw = true;

  while (1) {
    now += 1 + tick_wait();

    send_joystick_data();

    struct CAN_Message msg_incoming = can_wait_receive();
    switch (msg_incoming.ID) {
    case MSG_IR_BLOCKED_ID:
      is_ir_blocked = msg_incoming.data[0];
      break;
    }

    if (is_ir_blocked) {
      is_ir_blocked_strikes += 1;
    } else {
      is_ir_blocked_strikes = 0;
    }

    if (is_ir_blocked_strikes >= 2) {
      break;
    }

    if (now >= next_update_score) {
      next_update_score += SECONDS(1);
      *score += 1;
      redraw = true;
    }

    if (redraw) {
      game_draw(*score, false);
      redraw = false;
    }
  }

  send_game_data(false);
  tick_stop();
}

void game_loop() {
  int lives = 3;
  int score = 0;

  set_lights(lives);

  while (lives > 0) {
    game_round(&score);
    lives -= 1;
    set_lights(lives);

    game_draw(score, lives > 0);

    _delay_ms(1000);
  }
}

void game_start() {
  send_game_data(false);
  printf("starting game...");

  game_loop();

  printf("game finished...");
}
