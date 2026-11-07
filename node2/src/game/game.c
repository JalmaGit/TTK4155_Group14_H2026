#include "can.h"
#include "ir.h"
#include "msgs.h"
#include "serialize.h"
#include "servo.h"
#include "time.h"
#include <stdint.h>
#include <stdio.h>

int counter = 0;
void game_loop() {
  CanMsg msg;
  Buttons buttons;

  if (can_rx(&msg)) {               // returns 0 if no msg ready
    if (msg.id == MSG_USER_IO_ID) { // id=7 is joystick
      // printf("CAN ID: %u, length: %u, Y: %u\r\n", msg.id, msg.length,
      // msg.byte[1]);
      uint8_t servo_position = msg.byte[1];
      servo_move(servo_position);

      buttons.right = msg.byte[2];
      buttons.left = msg.byte[3];
      buttons.nav = msg.byte[4];
    }
  }

  time_spinFor(msecs(20));
  counter += 1;

  if (counter % 10 == 0) {
    printf("ADC: %d\r\n", read_adc());
  }

  // Send data
  CanMsg msg_tx;
  msg_tx.id = MSG_IR_BLOCKED_ID;
  msg_tx.length = 1;
  msg_tx.byte[0] = is_ir_blocked();
  can_tx(msg_tx);
}
