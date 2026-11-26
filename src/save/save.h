#ifndef SAVE_MODULE_H
#define SAVE_MODULE_H

#include <stdbool.h>
#include "../game.h"

int save_game(const Player *p);

bool load_game(Player *p);

void delete_save(void);

#endif
