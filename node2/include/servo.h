#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

typedef enum{
    stay,
    left,
    right
} Direction;

void servo_init();
void servo_move(uint8_t joystick_value);
void servo_go_left();
void servo_go_right();
void servo_center();


#endif //SERVO_H