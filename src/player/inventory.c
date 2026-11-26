#include <stdio.h>
#include <stdbool.h>
#include "inventory.h"
#include "../items/items.h"
#include "../ui/ui.h"


// Add an item if there is enough space in the inventory
bool inv_add(Player *p, Item it) {
    if (p->itemCount >= MAX_ITEMS) return false;

    // add the item at the end and increase the item count
    p->inventory[p->itemCount++] = it;
    return true;
}

// Remove an item and move the remaining items to fill the empty place
bool inv_remove(Player *p, int idx) {
    if (idx < 0 || idx >= p->itemCount) return false;

    // correct id of all next items
    for (int i = idx; i < p->itemCount - 1; i++)
        p->inventory[i] = p->inventory[i + 1];

    p->itemCount--;
    return true;
}


// Drink a potion, but only if there is something to heal
static bool use_potion(Player *p, int idx, char *msg, size_t n) {
    Item it = p->inventory[idx];

    if (p->hp >= p->maxHp) {
        snprintf(msg, n, "Mas plne HP.");
        return false; // nothing was used
    }

    // restore HP but do not go over the maximum
    p->hp += it.value;
    if (p->hp > p->maxHp) p->hp = p->maxHp;

    inv_remove(p, idx);
    snprintf(msg, n, "Vypil si %s. HP %d/%d.", it.name, p->hp, p->maxHp);
    return true;
}

// Equips the item from slot idx into a weapon or armor slot
static void equip(Player *p, int idx, Item *slot, int *has_item, int *stat) {
    Item it = p->inventory[idx];

    if (*has_item) {
        // the old item goes back to the inventory instead of the new one
        *stat -= slot->value;
        p->inventory[idx] = *slot;
    } else {
        inv_remove(p, idx);
        *has_item = 1;
    }

    *slot = it;
    *stat += it.value;
}

// Use a potion or equip an item
static bool use_item(Player *p, int idx, char *msg, size_t n) {
    Item it = p->inventory[idx];

    switch (it.type) {
        case ITEM_POTION: return use_potion(p, idx, msg, n);
        case ITEM_WEAPON: equip(p, idx, &p->weapon, &p->hasWeapon, &p->atk); break;
        case ITEM_ARMOR:  equip(p, idx, &p->armor, &p->hasArmor, &p->def);   break;
        default: return false;
    }

    snprintf(msg, n, "Nasadil si %s.", it.name);
    return true;
}

// Remove the selected item from the inventory
static void drop_item(Player *p, int idx, char *msg, size_t n) {
    snprintf(msg, n, "Zahodil si %s.", p->inventory[idx].name);
    inv_remove(p, idx);
}


static void draw_inventory(const Player *p, int cur, const char *msg) {
    ui_clear_screen();
    printf("=== INVENTAR (%d/%d) ===\n\n", p->itemCount, MAX_ITEMS);

    // show if inventory is empty
    if (p->itemCount == 0) printf("  (prazdny)\n");

    // show items and their effects, %-18s is a text with len>=18 aligned to the left
    for (int i = 0; i < p->itemCount; i++) {
        char effect[32];
        item_describe(&p->inventory[i], effect, sizeof(effect));

        
        printf(" %c %-18s %-8s %s\n",
            i == cur ? '>' : ' ',                   // show current item
            p->inventory[i].name,                   // name
            item_type_name(p->inventory[i].type),   // type
            effect                                  // ATK/DEF/HEAL
        );
    }

    // show equipped items and current stats
    printf("\nZbran:    %s\n", p->hasWeapon ? p->weapon.name : "-");
    printf("Brnenie:  %s\n", p->hasArmor ? p->armor.name : "-");
    printf("Attack %d | Defence %d | HP %d/%d\n", p->atk, p->def, p->hp, p->maxHp);

    printf("\n%s\n", msg);
    printf("W/S = vyber | E = pouzit/nasadit | X = zahodit | I/Q = zavriet\n");
}


// Inventory item management, using and equipping items.Returns how many items were used
int inv_menu(Player *p) {
    int cur = 0;
    int used = 0;
    char msg[96] = "";

    for (;;) {
        draw_inventory(p, cur, msg);
        msg[0] = '\0'; // clear the old message

        int key = ui_get_key();

        if (key == 'i' || key == 'q') return used;

        if (p->itemCount > 0 && key == 'e') used += use_item(p, cur, msg, sizeof(msg));
        else if (p->itemCount > 0 && key == 'x') drop_item(p, cur, msg, sizeof(msg));
        else cur = ui_move_selection(key, cur, p->itemCount);

        // the list may have become shorter
        if (cur >= p->itemCount) cur = p->itemCount > 0 ? p->itemCount - 1 : 0;
    }
}
