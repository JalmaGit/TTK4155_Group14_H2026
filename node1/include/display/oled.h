// OLED model MCOT128064H1V-WM with driver IC SSD1309

#ifndef OLED_H_
#define OLED_H_

#include <stdint.h>

#define WIDTH_PX 128
#define HEIGHT_PX 64

#define PAGES 8
#define PAGE_SIZE 128

// One page represents 8x128 bits/dots.

void oled_init(void);

void oled_reset(void);

void oled_clear_screen(void);

void oled_home(void);

void oled_goto_line(uint8_t line);

void oled_clear_line(uint8_t line);

void oled_pos(uint8_t row, uint8_t col);

void oled_write_data(char);

void oled_test(void);

// Char/String Writing
void oled_write_char_large(char c);
void oled_write_char_medium(char c);
void oled_write_char_small(char c);

void oled_write_str_large(const char *str);
void oled_write_str_medium(const char *str);
void oled_write_str_small(const char *str);

#endif // OLED_H_
