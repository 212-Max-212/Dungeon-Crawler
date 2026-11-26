#ifndef UI_H
#define UI_H

#include <stdbool.h>
#include "../game.h"

// Screen
void ui_init(void);
void ui_clear_screen(void);
void ui_draw_hud(const Player *p);
void ui_pause(void);
void start_plot(void);
void finish_game(const Player *p);

// Keyboard
int ui_get_key(void);

// Shared input helpers
int ui_choose(int max, bool allow_cancel);
int ui_move_selection(int key, int selected, int count);
bool ui_confirm(const char *question);

#endif
