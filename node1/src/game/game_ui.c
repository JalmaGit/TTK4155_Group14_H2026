#include "game/game_ui.h"
#include "display/oled.h"
#include <stdio.h>

char buffer[50];
void game_draw(int score) {
  oled_clear_screen();

  oled_pos(0, 0);
  oled_write_str_large("Playing!");

  oled_pos(1, 0);
  sprintf(buffer, sizeof(buffer), "Score %d", score);
  oled_write_str_medium(buffer);
}
