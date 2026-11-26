#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "combat.h"
#include "../player/inventory.h"
#include "../save/save.h"
#include "../player/skills.h"
#include "../ui/ui.h"


// Damage = attack - defense, but at least 1 if atk > def so every fight can end
int calc_damage(int atk, int def) {
    int atk_min = atk/2;
    int def_min = (def/3)*2;

    int attack  = atk > 0 ? (rand() % (atk-atk_min)) + atk_min : 1;
    int defense = def > 0 ? (rand() % (def-def_min)) + def_min : 0;

    int d = attack - defense;
    return d < 1 ? (defense >= attack ? 0 : 1) : d;
}

// Enemy with no HP left is defeated
static bool enemy_defeated(Enemy *e) {
    if (e->hp > 0) return false;

    e->hp = 0;
    return true;
}

// Basic attack, also regenerates a little mana
static int basic_attack(Player *p, Enemy *e, Battle *b) {
    int d = calc_damage(p->atk, e->def);

    e->hp -= d;
    p->mana += 1;

    if (p->mana > p->maxMana) p->mana = p->maxMana;
    snprintf(b->logA, sizeof(b->logA), "Zasiahol si %s za %d.", e->name, d);

    return enemy_defeated(e) ? COMBAT_WIN : TURN_SPENT;
}


// Open the skills menu and use the selected skill
static int use_skill(Player *p, Enemy *e, Battle *b) {
    int id = skill_menu(p);
    if (id < 0) return TURN_AGAIN;

    // if the skill cannot be used, the turn is not spent
    if (!skill_use(id, p, e, &b->bs, b->logA, sizeof(b->logA))) return TURN_AGAIN;

    return enemy_defeated(e) ? COMBAT_WIN : TURN_SPENT;
}

// Open the inventory, if nothing was used the turn is not spent
static int open_inventory(Player *p, Battle *b) {
    if (inv_menu(p) == 0) return TURN_AGAIN;

    snprintf(b->logA, sizeof(b->logA), "Pouzil si predmet.");
    return TURN_SPENT;
}


static int load_saved_game(Player *p, Battle *b) {
    if (load_game(p)) return COMBAT_LOADED;

    snprintf(b->logA, sizeof(b->logA), "Ziadne ulozenie na nacitanie.");
    return TURN_AGAIN;
}


// Read one key and run the matching action
static int player_turn(Player *p, Enemy *e, Battle *b) {
    switch (ui_get_key()) {
        case '1': return basic_attack(p, e, b);
        case '2': return COMBAT_RUN;
        case '3': return use_skill(p, e, b);
        case 'l': return load_saved_game(p, b);
        case 'i': return open_inventory(p, b);
        case 'q': return ui_confirm("Naozaj skoncit?") ? COMBAT_QUIT : TURN_AGAIN;
        case 'p': return COMBAT_WIN;
        default:  return TURN_AGAIN;
    }
}

// The enemy attacks (unless the player dodges); returns COMBAT_LOSE or TURN_SPENT
static int enemy_turn(Player *p, const Enemy *e, Battle *b) {
    if (b->bs.dodge) {
        b->bs.dodge = 0; // dodge is used up by one attack
        snprintf(b->logB, sizeof(b->logB), "Uhol si sa utoku: %s.", e->name);
    } else {
        // add shield bonus to defense if the shield is still active
        int def = p->def + (b->bs.shieldTurns > 0 ? SHIELD_BONUS : 0);
        int d = calc_damage(e->atk, def);

        p->hp -= d;
        snprintf(b->logB, sizeof(b->logB), "%s ta zasiahol za %d.", e->name, d);

        if (p->hp <= 0) {
            p->hp = 0;
            return COMBAT_LOSE;
        }
    }

    // reduce shield duration after the enemy's turn
    if (b->bs.shieldTurns > 0) b->bs.shieldTurns--;
    return TURN_SPENT;
}


static void draw_battle(const Player *p, const Enemy *e, const Battle *b) {
    ui_clear_screen();

    printf("=== BOJ ===\n\n");

    printf("%s   HP %d/%d   MP %d/%d\n", "Hrdina", p->hp, p->maxHp, p->mana, p->maxMana); // player info
    printf("%s   HP %d\n\n", e->name, e->hp);                                             // enemy info

    // show the last actions if there are any
    if (b->logA[0]) printf("%s\n", b->logA);
    if (b->logB[0]) printf("%s\n", b->logB);

    // show all abilities to do
    printf("\n[1] Utok  [2] Utek  [3] Schopnosti  [I] Inventar  [L] Nacitat  [Q] Koniec\n");
}


// Run an encounter and return one of the COMBAT_* results
int combat_start(Player *p, Enemy *e) {
    Battle b = {"", "", {0, 0}};
    int result = 0;

    for (;;) {
        draw_battle(p, e, &b);
        b.logA[0] = b.logB[0] = '\0'; // clear the old messages

        result = player_turn(p, e, &b);
    
        if (result == TURN_AGAIN) continue;
        if (result != TURN_SPENT) return result;

        result = enemy_turn(p, e, &b);
        if (result != TURN_SPENT) return result;
    }
}
