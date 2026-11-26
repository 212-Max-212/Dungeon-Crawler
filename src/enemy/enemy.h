#ifndef ENEMY_MODULE_H
#define ENEMY_MODULE_H

#include "../game.h"

#define ENEMY_KINDS 7

Enemy enemy_make(int id); // id must be in the range 0..ENEMY_KINDS-1

#endif
