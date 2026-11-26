#include <stdio.h>
#include <stdbool.h>
#include "skills.h"
#include "../combat/combat.h"
#include "../ui/ui.h"


static const Skill SKILLS[SKILL_COUNT] = {
    // name,      level, mana cost, description
    {"Stit",        1,       6,     "+5 Defence na 3 kola"          },
    {"Liecenie",    2,       8,     "lieci 20 HP"                   },
    {"Silny uder",  3,       5,     "dvojnasobny Attack"            },
    {"Ohnova gula", 4,       12,    "Attack + 10, ignoruje Defence" },
    {"Uhyb",        5,       7,     "vyhnes sa dalsiemu utoku"      }
};


// Checks all requirements before mana is spent or a turn is used
// If something is missing, the reason is written to msg
static bool can_use_skill(const Player *p, int id, char *msg, size_t n) {
    if (id < 0 || id >= SKILL_COUNT) {
        snprintf(msg, n, "Neplatna schopnost.");
        return false;
    }
    if (!is_skill_unlocked(p, id)) {
        snprintf(msg, n, "Tuto schopnost este nemas.");
        return false;
    }
    if (p->mana < SKILLS[id].manaCost) {
        snprintf(msg, n, "Malo many (treba %d).", SKILLS[id].manaCost);
        return false;
    }
    if (id == SKILL_HEAL && p->hp >= p->maxHp) {
        snprintf(msg, n, "Mas plne HP.");
        return false;
    }
    return true;
}

// A new one unlocks with every level
bool is_skill_unlocked(const Player *p, int id) {
    return p->level >= SKILLS[id].unlockLevel;
}

// The effect of every skill
static void apply_skill(int id, Player *p, Enemy *e, BattleState *bs, char *msg, size_t n) {
    int d;

    switch (id) {
        case SKILL_SHIELD:                                                  // increase defense for the next 3 turns
            bs->shieldTurns = 3;
            snprintf(msg, n, "Stit: +%d DEF na 3 kola.", SHIELD_BONUS);
            break;
        case SKILL_HEAL:                                                    // restore 20 HP without exceeding max HP
            p->hp += 30;
            if (p->hp > p->maxHp) p->hp = p->maxHp;
            snprintf(msg, n, "Vylieci si sa. HP %d/%d.", p->hp, p->maxHp);
            break;
        case SKILL_POWER:                                                   // deal double attack damage, reduced by enemy defense
            d = calc_damage(p->atk * 2, e->def);
            e->hp -= d;
            snprintf(msg, n, "Silny uder zasiahol %s za %d.", e->name, d);
            break;
        case SKILL_FIREBALL:                                                // deal damage that ignores enemy defense
            d = p->atk + 10;
            e->hp -= d;
            snprintf(msg, n, "Ohnova gula zasiahla %s za %d.", e->name, d);
            break;
        case SKILL_DODGE:                                                   // avoid the next enemy attack
            bs->dodge = 1;
            snprintf(msg, n, "Pripravil si sa uhnut utoku.");
            break;
    }
}

bool skill_use(int id, Player *p, Enemy *e, BattleState *bs, char *msg, size_t n) {
    if (!can_use_skill(p, id, msg, n)) return false;

    p->mana -= SKILLS[id].manaCost; // spend mana after all checks pass
    apply_skill(id, p, e, bs, msg, n);
    return true;
}


static void draw_skills(const Player *p) {
    ui_clear_screen();
    printf("=== SCHOPNOSTI (MP %d/%d) ===\n\n", p->mana, p->maxMana);

    for (int i = 0; i < SKILL_COUNT; i++) {
        if (is_skill_unlocked(p, i))
            printf(" [%d] %-12s %2d MP  %s\n",
                i + 1,                // level
                SKILLS[i].name,       // name
                SKILLS[i].manaCost,   // mana cost
                SKILLS[i].desc        // description
            );
        else
            printf("  -  %-12s zamknute (level %d) %s\n",
                SKILLS[i].name,       // name
                SKILLS[i].unlockLevel,// level
                SKILLS[i].desc        // description
            );
    }

    printf("\n[1-5] = pouzit | Q = spat\n");
}


// Returns the id of the chosen skill, or -1 when the menu is closed.
int skill_menu(const Player *p) {
    for (;;) {
        draw_skills(p);

        int choice = ui_choose(SKILL_COUNT, true);
        if (choice == -1) return -1;

        // a locked skill cannot be chosen
        if (is_skill_unlocked(p, choice - 1)) return choice - 1;
    }
}
