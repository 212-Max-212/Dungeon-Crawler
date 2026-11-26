#include <stdio.h>
#include "encounter.h"
#include "combat.h"
#include "../map/map.h"
#include "../enemy/enemy.h"
#include "../player/player.h"
#include "../ui/ui.h"

// A victory gives XP, heals a little and refills a bit of mana (a level up refills everything).
static void apply_victory(Player *p, const Enemy *e, char *msg, size_t size) {
    p->hp += 5;
    if (p->hp > p->maxHp) p->hp = p->maxHp;

    p->mana += 5;
    if (p->mana > p->maxMana) p->mana = p->maxMana;

    int levels = player_add_xp(p, e->xpReward);

    snprintf(msg, size, "Porazil si %s! +%d XP.%s",
        e->name,
        e->xpReward,
        levels > 0 ? " LEVEL UP!" : ""
    );
}

static int game_over(void) {
    ui_clear_screen();
    printf("Zomrel si. KONIEC HRY.\n");
    ui_pause();
    return ENC_END;
}


int encounter_run(Player *p, int x, int y, char *msg, size_t msg_size) {
    int enemy_id = map_enemy_id(map_get_pos(x, y));
    Enemy e = enemy_make(enemy_id);

    int result = combat_start(p, &e);

    switch (result) {
        case COMBAT_LOSE:   return game_over();
        case COMBAT_QUIT:   return ENC_END;
        case COMBAT_RUN:    snprintf(msg, msg_size, "Utiekol si pred %s.", e.name); return ENC_STAY;
        case COMBAT_LOADED: snprintf(msg, msg_size, "Hra nacitana."); return ENC_STAY;
        default:            apply_victory(p, &e, msg, msg_size); break;
    }

    // the enemy is gone
    map_set_pos(x, y, '.');
    return ENC_MOVE;
}
