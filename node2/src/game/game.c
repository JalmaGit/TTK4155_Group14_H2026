#include <stdint.h>
#include "can.h"
#include "servo.h"
#include <stdio.h>

void game_loop() {
    CanMsg msg;

    if (can_rx(&msg)) { // returns 0 if no msg ready
        if (msg.id == 7) { // id=7 is joystick
            printf("CAN ID: %u, length: %u, Y: %u\r\n", msg.id, msg.length, msg.byte[1]);
            uint8_t servo_position = msg.byte[1];
            servo_move(servo_position);
        }
    }
}
