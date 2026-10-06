#include "display/oled.h"
#include "display/fonts.h"
#include "display/ssd1309.h"
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

void oled_init(void) {
  ssd1309_init();
  display_on();
  addressing_mode_page();
}

void oled_reset(void) { oled_init(); }

void oled_write_data(char data) { send_data(data); }

void oled_write_char_large(char c) {
  for (int i = 0; i < 8; i++) {
    send_data(get_glyph8_byte(c, i));
  }
}
void oled_write_char_medium(char c) {
  for (int i = 0; i < 5; i++) {
    send_data(get_glyph5_byte(c, i));
  }
}

void oled_write_char_small(char c) {
  for (int i = 0; i < 4; i++) {
    send_data(get_glyph4_byte(c, i));
  }
}

void oled_write_str_large(const char *str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
    oled_write_char_large(str[i]);
  }
}

void oled_write_str_medium(const char *str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
    oled_write_char_medium(str[i]);
  }
}

void oled_write_str_small(const char *str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
    oled_write_char_small(str[i]);
  }
}

void oled_pos(uint8_t row, uint8_t col) {
  set_page(row);
  set_column(col);
}

void oled_clear_screen() {
  for (int i = 0; i < 8; i++) {
    oled_clear_line(i);
  }
}

void oled_goto_line(uint8_t line) { set_page(line); }

void oled_clear_line(uint8_t line) {
  set_page(line);
  for (int i = 0; i < 128; i++) {
    send_data(0);
  }
}

void oled_test(void) {
  // Draw "Test"
  oled_clear_screen();

  oled_pos(0, 0);
  oled_write_str_medium("Test0");

  oled_pos(4, 4);
  oled_write_str_medium("Test4");

  oled_pos(5, 0);
  oled_write_str_large("Test8");

  while (1) {
    _delay_ms(100);
  }
}
