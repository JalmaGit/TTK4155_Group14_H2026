#include "can.h"
#include "ir.h"
#include "msgs.h"
#include "serialize.h"
#include "servo.h"
#include "solenoid.h"
#include "time.h"
#include <stdint.h>
#include <stdio.h>
#include "sam.h"
#include "motor.h"

void game_loop() {
  CanMsg msg;
  Buttons buttons;

  time_spinFor(msecs(50));

  uint32_t position_counter = TC2->TC_CHANNEL[0].TC_CV;
  printf("Position: %d\r\n", position_counter);

  if (can_rx(&msg)) {
    if (msg.id == MSG_USER_IO_ID) {
      uint8_t servo_position = msg.byte[1];
      servo_move(servo_position);

      uint8_t motor_value = msg.byte[0];
      motor_move_pid(motor_value);

      buttons.right = msg.byte[2];
      buttons.left = msg.byte[3];
      buttons.nav = msg.byte[4];

      solenoid_set(buttons.R6);
    }
  }

  // Send data
  CanMsg msg_tx;
  msg_tx.id = MSG_IR_BLOCKED_ID;
  msg_tx.length = 1;
  msg_tx.byte[0] = is_ir_blocked();
  can_tx(msg_tx);

  time_spinFor(msecs(20));
}
