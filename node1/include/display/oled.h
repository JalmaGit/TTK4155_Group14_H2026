// OLED model MCOT128064H1V-WM with driver IC SSD1309
#ifndef OLED_H_
#define OLED_H_

#include <stdint.h>

#define WIDTH_PX 128
#define HEIGHT_PX 64

#define PAGES 8
#define PAGE_SIZE 128

// One page represents 8x128 bits/dots.

void oled_init();

void oled_reset();

void oled_home();

void oled_goto_line(uint8_t line);

void oled_clear_line(uint8_t line);

void oled_pos(uint8_t row, uint8_t col);

/// TODO: Remember to make volatile
void oled_write_data(char);

void oled_print(char *);

void oled_set_brightness(uint8_t level);

#endif // OLED_H_
