#include "can.h"
#include "ir.h"
#include "motor.h"
#include "msgs.h"
#include "pid.h"
#include "sam.h"
#include "serialize.h"
#include "servo.h"
#include "solenoid.h"
#include "time.h"
#include <stdint.h>
#include <stdio.h>

float set_point = 0.5;

void game_loop() {
  CanMsg msg;
  Buttons buttons;

  printf("Position: %f, set_point: %f\r\n", get_motor_position_normalized(),
         set_point);

  if (can_rx(&msg)) {
    if (msg.id == MSG_USER_IO_ID) {
      uint8_t servo_position = msg.byte[1];
      servo_move(servo_position);

      float motor_value = byte_to_float(msg.byte[0], -1, 1);

      if (motor_value < 0.2 && motor_value > -0.2) {
        motor_value = 0;
      }

      set_point += motor_value * 0.05;

      if (set_point > 1) {
        set_point = 1;
      } else if (set_point < 0) {
        set_point = 0;
      }

      buttons.right = msg.byte[2];
      buttons.left = msg.byte[3];
      buttons.nav = msg.byte[4];

      solenoid_set(buttons.R6);
    } else if (msg.id == MSG_GAME_DATA_ID) {
      bool is_playing = msg.byte[0];

      if (!is_playing) {
        printf("NOT PLAYING\r\n");
        init_PID(MOTOR_P, MOTOR_I, MOTOR_D);
      }
    }
  }

  motor_move_pid(set_point);

  // Send data
  CanMsg msg_tx;
  msg_tx.id = MSG_IR_BLOCKED_ID;
  msg_tx.length = 1;
  msg_tx.byte[0] = is_ir_blocked();
  can_tx(msg_tx);

  time_spinFor(msecs(20));
}
