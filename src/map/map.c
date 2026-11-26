#include <stdio.h>
#include <string.h>
#include "map.h"
#include "map_data.h"
#include "../enemy/enemy.h"

// Map - 6 maps of 20x10, the player walks on them and enemies and items lie on their tiles
static MapState world;

// Start from a clean world: copy the template and stand in the first location
void map_init(void) {
    memcpy(world.tiles, MAP_TEMPLATE, sizeof(MAP_TEMPLATE));
    world.loc = 0;
}

bool map_is_walkable(int x, int y) {
    return map_get_pos(x, y) != '#';
}


char map_get_pos(int x, int y) {
    if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H) return '#';
    return world.tiles[world.loc][y][x];
}

void map_set_pos(int x, int y, char c) {
    if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H) return;
    world.tiles[world.loc][y][x] = c;
}


// Move to the next location; returns false when this is already the last one
bool map_next_location(void) {
    if (world.loc + 1 >= LOC_COUNT) return false;

    world.loc++;
    return true;
}

const char *map_location_name(void) {
    return MAP_NAMES[world.loc];
}


int map_enemy_id(char tile) {
    return tile - '1';
}

bool map_is_enemy(char tile) {
    return tile >= '1' && tile < '1' + ENEMY_KINDS;
}

bool map_is_item(char tile) {
    return tile >= 'a' && tile <= 'o';
}


void map_get_state(MapState *out) {
    *out = world;
}

void map_set_state(const MapState *in) {
    world = *in;
}


// The tiles that are not the player, an enemy or an item
static void draw_other_tile(char pos) {
    switch (pos) {
        case '#': printf("\x1b[90m#\x1b[0m"); break; // wall
        case '>': printf("\x1b[94m>\x1b[0m"); break; // portal
        case 'X': printf("\x1b[92mX\x1b[0m"); break; // exit
        default:  putchar(pos); break;               // empty
    }
}

static void draw_tile(const Player *p, int x, int y) {
    char pos = map_get_pos(x, y);

    if (x == p->x && y == p->y) printf("\x1b[33m@\x1b[0m"); // player
    else if (map_is_enemy(pos)) printf("\x1b[31mE\x1b[0m"); // enemy
    else if (map_is_item(pos)) printf("\x1b[93m!\x1b[0m");  // item
    else draw_other_tile(pos);
}

void map_draw(const Player *p) {
    int len = strlen(MAP_NAMES[world.loc]);
    int padding = (MAP_W - len-6) / 2;      // 6 is ==  ==
    
    // draw name of location
    printf("\x1b[1m%*s== %s ==\x1b[0m\n", padding, "", MAP_NAMES[world.loc]);

    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) draw_tile(p, x, y);
        putchar('\n');
    }

    // draw keys
    printf("@ ty | E nepriatel | ! predmet | > dalsia lokacia | X vychod\n");
}
