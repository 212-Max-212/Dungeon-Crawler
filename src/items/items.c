#include <stdio.h>
#include "items.h"

// 3 types of items (weapon, armor, potion) with 5 items of each type
static const Item ITEM_TABLE[ITEM_KINDS] = {
    //    name,            type,    value
    {"Drevena palica",  ITEM_WEAPON, 2  },  // a
    {"Zelezny mec",     ITEM_WEAPON, 4  },  // b
    {"Ocelovy mec",     ITEM_WEAPON, 6  },  // c
    {"Runovy mec",      ITEM_WEAPON, 9  },  // d
    {"Dracy mec",       ITEM_WEAPON, 13 },  // e

    {"Kozena vesta",    ITEM_ARMOR,  1  },  // f
    {"Kolcuga",         ITEM_ARMOR,  2  },  // g
    {"Zelezny platnac", ITEM_ARMOR,  3  },  // h
    {"Platova zbroj",   ITEM_ARMOR,  5  },  // i
    {"Dracie supiny",   ITEM_ARMOR,  8  },  // j

    {"Maly napoj",      ITEM_POTION, 20 },  // k
    {"Liecivy napoj",   ITEM_POTION, 40 },  // l
    {"Velky napoj",     ITEM_POTION, 70 },  // m
    {"Elixir zivota",   ITEM_POTION, 100},  // n
    {"Mastny elixir",   ITEM_POTION, 999}   // o
};

// Names shown in the inventory
static const char *TYPE_NAMES[3] = {"zbran", "brnenie", "elixir"};

// Returns a copy of an item from the table
Item item_make(int id) {
    Item empty = {"Neznamy predmet", ITEM_POTION, 0};

    if (id < 0 || id >= ITEM_KINDS) return empty;
    return ITEM_TABLE[id];
}

// Create a short description based on the item type
void item_describe(const Item *it, char *out, size_t size) {
    if (it->type == ITEM_WEAPON) snprintf(out, size, "Attack +%d", it->value);
    else if (it->type == ITEM_ARMOR) snprintf(out, size, "Defence +%d", it->value);
    else snprintf(out, size, "Heal %d HP", it->value);
}

const char *item_type_name(int type) {
    return (type >= 0 && type < 3) ? TYPE_NAMES[type] : "?";
}
