#ifndef INVENTORY_MODULE_H
#define INVENTORY_MODULE_H

#include <stdbool.h>
#include "../game.h"

// Inventory
bool inv_add(Player *p, Item it);    // false = full inventory
bool inv_remove(Player *p, int idx); // false = bad index

int inv_menu(Player *p);             // returns how many items were used

#endif
