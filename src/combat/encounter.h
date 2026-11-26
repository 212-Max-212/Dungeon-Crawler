#ifndef ENCOUNTER_MODULE_H
#define ENCOUNTER_MODULE_H

#include "../game.h"

// What the main loop does after an encounter
enum {
    ENC_STAY, // the player stays where he was (ran away, loaded a game)
    ENC_MOVE, // the enemy is gone, the player steps onto its tile
    ENC_END   // the game ends (the player died or quit)
};

// The player stepped onto the enemy at (x, y): fight and give rewards
// A short message for the main screen is written to msg
int encounter_run(Player *p, int x, int y, char *msg, size_t msg_size);

#endif
