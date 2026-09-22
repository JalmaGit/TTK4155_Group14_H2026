// OLED Driver
#ifndef OLED_H_
#define OLED_H_

#include <stdint.h>

void send_command(uint8_t command);

// Commands //

void display_on(void);

void addressing_mode_horizontal(void);
void addressing_mode_vertical(void);
void addressing_mode_page(void);

void set_column_address(uint8_t start, uint8_t end);

// Note: Automatic address increment on data commands

#endif // OLED_H_
