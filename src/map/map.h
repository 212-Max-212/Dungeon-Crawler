#ifndef MAP_MODULE_H
#define MAP_MODULE_H

#include <stdbool.h>
#include "../game.h"

#define MAP_W 20
#define MAP_H 10
#define LOC_COUNT 6

// The player always starts a new location here
#define START_X 1
#define START_Y 1

// World is every location + the current one (this is what Save/Load stores)
typedef struct {
    char tiles[LOC_COUNT][MAP_H][MAP_W + 1];
    int loc;                                    // current location
} MapState;

void map_init(void);
bool map_is_walkable(int x, int y);

char map_get_pos(int x, int y);
void map_set_pos(int x, int y, char c);

bool map_next_location(void);
const char *map_location_name(void);

// What a tile is
int map_enemy_id(char tile);
bool map_is_enemy(char tile); // digits '1'..'7', the digit minus '1' is the enemy id
bool map_is_item(char tile);  // letters 'a'..'o', the letter minus 'a' is the item id

// used by Save/Load
void map_get_state(MapState *out);
void map_set_state(const MapState *in);

void map_draw(const Player *p);

#endif
