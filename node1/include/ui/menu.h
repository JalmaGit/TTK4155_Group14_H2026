// Repsonsible for oled menu logic.

#ifndef MENU_H_
#define MENU_H_

#define MENU_BACK (-1)

typedef struct menu menu_t;

typedef struct {
  const char *label;
  int id;
} menu_item_t;

int menu_run(const char *title, const menu_item_t *items, int count,
             int *selected);

#endif // MENU_H_
