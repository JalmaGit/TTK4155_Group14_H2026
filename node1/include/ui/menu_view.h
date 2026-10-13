/// This module declares the drawing utilities for the menu.

#ifndef MENU_VIEW_H_
#define MENU_VIEW_H_

#include "ui/menu.h"
#include <stdint.h>

void menu_clear(void);

void menu_draw(const char *title, const menu_item_t *items, uint8_t count,
               uint8_t selected);

#endif // MENU_VIEW_H_
