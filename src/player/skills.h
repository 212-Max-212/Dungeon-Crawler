#ifndef SKILLS_MODULE_H
#define SKILLS_MODULE_H

#include <stdbool.h>
#include "../game.h"
#include "../combat/combat.h"

#define SKILL_COUNT 5
#define SHIELD_BONUS 5

// skills
enum{
    SKILL_SHIELD,
    SKILL_HEAL,
    SKILL_POWER,
    SKILL_FIREBALL,
    SKILL_DODGE
};

typedef struct {
    const char *name;
    int unlockLevel;  // Level needed to unlock the skill
    int manaCost;
    const char *desc; // Description shown in the menu
} Skill;

// If skill was used (turn is spent).
// If refused (reason is written to msg, turn is NOT spent)
bool skill_use(int id, Player *p, Enemy *e, BattleState *bs, char *msg, size_t n);

bool is_skill_unlocked(const Player *p, int id);

int skill_menu(const Player *p);

#endif