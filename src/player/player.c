#include <string.h>
#include "player.h"

void player_init(Player *p) {
    memset(p, 0, sizeof(*p)); // prepare clear memory for structure
    p->maxHp = 50;
    p->hp = 50;

    p->atk = 8;
    p->def = 2;

    p->maxMana = 20;
    p->mana = 20;

    p->level = 1;

    p->x = 1;
    p->y = 1;
}

int player_xp_needed(const Player *p) {
    return p->level * 50;
}

// 5 levels, every level improves the stats and unlocks a new skill
// Adds XP and raises the level while there is enough XP (the maximum is level 5).
// Every new level improves the stats and refills HP and mana.
int player_add_xp(Player *p, int xp) {
    int gained = 0;
    p->xp += xp;

    while (p->level < MAX_LEVEL && p->xp >= player_xp_needed(p)) {
        p->xp -= player_xp_needed(p);
        p->level++;

        p->maxHp += 10;
        p->hp = p->maxHp;

        p->atk += 2;
        p->def += 1;

        p->maxMana += 5;
        p->mana = p->maxMana;

        gained++;
    }
    return gained;
}
