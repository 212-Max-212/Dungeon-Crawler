#include "enemy.h"

// Table of 7 enemies, the digit on the map is id + 1
static const Enemy ENEMY_TABLE[ENEMY_KINDS] = {
    // name,      hp, atk, def, xp reward
    {"Potkan",    15,  6,   3,      5  },   // 1
    {"Vlk",       20,  8,   8,      5  },   // 2
    {"Goblin",    30,  9,   7,      15 },   // 3
    {"Kostlivec", 20,  13,  6,      15 },   // 4
    {"Drak",      200, 18,  15,     0  },   // 5
    {"Pavuk",     35,  8,   4,      20 },   // 6
    {"Bandita",   75,  15,  7,      30 }    // 7
};

// Returns a copy of the enemy, so the original in the table stays the same
Enemy enemy_make(int id) {
    return (id >= 0 && id < ENEMY_KINDS) ? ENEMY_TABLE[id] : ENEMY_TABLE[0];
}
