#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "game.h"
#include "ui/ui.h"
#include "map/map.h"
#include "player/player.h"
#include "items/items.h"
#include "player/inventory.h"
#include "save/save.h"
#include "combat/encounter.h"


// Pick up an item when entering an item tile (a full inventory leaves it on the map).
static void pick_up_item(Player *p, int x, int y, char *msg, size_t size) {
    Item item = item_make(map_get_pos(x, y) - 'a');

    if (inv_add(p, item)) {
        map_set_pos(x, y, '.');
        snprintf(msg, size, "Zdvihol si: %s.", item.name);
    } else snprintf(msg, size, "Inventar je plny.");
}

// The portal leads to the next location, where the player always starts at START_X, START_Y.
static void enter_portal(Player *p, char *msg, size_t size) {
    if (!map_next_location()) return;

    p->x = START_X;
    p->y = START_Y;
    
    snprintf(msg, size, "Vstupil si do lokacie: %s.", map_location_name());
}

// Converts W/A/S/D to a step. Returns false for any other key.
static bool read_direction(int key, int *dx, int *dy) {
    *dx = 0;
    *dy = 0;

    switch (key) {
        case 'w': *dy = -1; return true;
        case 's': *dy = 1;  return true;
        case 'a': *dx = -1; return true;
        case 'd': *dx = 1;  return true;
        default:  return false;
    }
}

// What happens when the player steps onto a tile. Returns false when the game is over.
static bool step_onto(Player *p, int nx, int ny, char *msg, size_t size) {
    char tile = map_get_pos(nx, ny);

    if(tile == '.'){
        p->x = nx;
        p->y = ny;
        return true;
    }

    if (map_is_enemy(tile)) {
        int result = encounter_run(p, nx, ny, msg, size);

        if (result == ENC_END) return false;
        if (result == ENC_STAY) return true;
    }
    
    // the portal and the exit are not stepped on
    else if (tile == '>') { enter_portal(p, msg, size); return true; }
    else if (tile == 'X') { finish_game(p); return false; }
    else if (map_is_item(tile)) pick_up_item(p, nx, ny, msg, size);

    p->x = nx;
    p->y = ny;
    
    return true;
}

// Commands that do not move the player. Returns true when the key was a command.
static bool run_command(Player *p, int key, char *msg, size_t size) {
    switch (key) {
        case 'i': inv_menu(p); return true;
        case 'k': snprintf(msg, size, save_game(p) ? "Hra ulozena." : "Ulozenie zlyhalo."); return true;
        case 'l': snprintf(msg, size, load_game(p) ? "Hra nacitana." : "Ziadne ulozenie na nacitanie."); return true;
        case 'u': player_add_xp (p,player_xp_needed(p)); return true;
        default:  return false;
    }
}


// Shows the map, the stats and the last message (which is cleared after that).
static void draw_screen(const Player *p, char *msg) {
    ui_clear_screen();
    map_draw(p);
    ui_draw_hud(p);

    if (msg[0]) printf("\n%s\n", msg);
    msg[0] = '\0';
}

int main(void) {
    srand(time(NULL));
    
    Player player;
    char msg[128] = "";

    ui_init();
    map_init();
    player_init(&player);

    if(!load_game(&player)) start_plot(); // continue the saved game if there is one. If not, start the plot (the player is at level 1 with no items).

    // Main game loop
    for (;;) {
        draw_screen(&player, msg);

        int key = ui_get_key();
        if (key == 'q' && (ui_confirm("Naozaj chces odist?"))) break;
        if (run_command(&player, key, msg, sizeof(msg))) continue;
        
        int dx, dy;
        if (!read_direction(key, &dx, &dy)) continue;

        // check if the move is valid
        int nx = player.x + dx, ny = player.y + dy;
        if (!map_is_walkable(nx, ny)) continue;

        if (!step_onto(&player, nx, ny, msg, sizeof(msg))) break;
    }

    return 0;
}
