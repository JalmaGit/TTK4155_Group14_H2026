#include "can.h"
#include "ir.h"
#include "servo.h"
#include "time.h"
#include <stdint.h>
#include <stdio.h>
#include "sam.h"
#include "motor.h"

int counter = 0;
void game_loop() {
  CanMsg msg;

  if (can_rx(&msg)) {  // returns 0 if no msg ready
    if (msg.id == 7) { // id=7 is joystick
      // printf("CAN ID: %u, length: %u, Y: %u\r\n", msg.id, msg.length,
      // msg.byte[1]);
      uint8_t servo_new_position = msg.byte[1];
      servo_move(servo_new_position);

      uint8_t motor_value = msg.byte[0];
      motor_move_value(motor_value);
    }
  }

  uint32_t position_counter = TC2->TC_CHANNEL[0].TC_CV;
  printf("Position: %d\r\n", position_counter);

  time_spinFor(msecs(50));
  counter += 1;

  if (counter % 10 == 0) {
    printf("ADC: %d\r\n", read_adc());
  }
}
