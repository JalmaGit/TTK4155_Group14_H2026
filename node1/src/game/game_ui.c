#include "game/game_ui.h"
#include "display/oled.h"
#include <stdbool.h>
#include <stdio.h>

char buffer[50];
void game_draw(int score, bool is_respawning) {
  oled_clear_screen();

  oled_pos(0, 0);
  oled_write_str_large("Playing!");

  oled_pos(1, 0);
  snprintf(buffer, sizeof(buffer), "Score %d", score);
  oled_write_str_medium(buffer);

  if (is_respawning) {
    oled_pos(2, 0);
    oled_write_str_medium("Respawning...");
  }
}

void game_draw_ball_warning(void) {
  oled_clear_screen();

  oled_pos(0, 0);
  oled_write_str_large("Warning!");
  oled_pos(2, 0);
  oled_write_str_medium("Please remove ball...");
}

void game_draw_press_continue(void) {
  oled_clear_screen();

  oled_pos(0, 0);
  oled_write_str_medium("Press SR6 to continue...");
}
