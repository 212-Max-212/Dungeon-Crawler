#ifndef COMBAT_H
#define COMBAT_H

#include "../game.h"

// States of combat
enum {
    COMBAT_WIN,
    COMBAT_LOSE,
    COMBAT_RUN,
    COMBAT_LOADED,                      // the player loaded a saved game in the middle of the fight
    COMBAT_QUIT,

    // Results of player action that do not end the fight.
    TURN_AGAIN = -3,                    // nothing happened, the player chooses again
    TURN_SPENT = -2                     // the turn was used, now the enemy acts
};

// Effects that last only for one fight
typedef struct{
    int shieldTurns; // enemy turns left with the DEF bonus
    int dodge;       // 1 = next enemy attack is avoided
} BattleState;


// Everything that exists only for one fight
typedef struct {
    char logA[96];   // line about the player's action
    char logB[96];   // line about the enemy's action

    BattleState bs;  // effects Shield / Dodge
} Battle;

int calc_damage(int atk, int def);      // damage = attack - defense, but at least 1 if atk > def
int combat_start(Player *p, Enemy *e);

#endif
