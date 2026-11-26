#ifndef ITEMS_MODULE_H
#define ITEMS_MODULE_H

#include "../game.h"

#define ITEM_KINDS 15

Item item_make(int id);                                     // id is in range 0..ITEM_KINDS-1

void item_describe(const Item *it, char *out, size_t size); // short effect text, e.g. "Attack +4"

const char *item_type_name(int type);

#endif
