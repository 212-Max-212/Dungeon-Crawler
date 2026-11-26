#ifndef PLAYER_H
#define PLAYER_H

#include "../game.h"

#define MAX_LEVEL 5

void player_init(Player *p);

int player_xp_needed(const Player *p);
int player_add_xp(Player *p, int xp); // add xp and returns how many levels were gained

#endif
