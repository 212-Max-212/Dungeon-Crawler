#ifndef GAME_H
#define GAME_H

#define MAX_ITEMS 5

enum {
    ITEM_WEAPON,
    ITEM_ARMOR,
    ITEM_POTION
};

typedef struct {
    char name[32];
    int type;                   // ITEM_*
    int value;                  // attack / defence bonus, or the amount of HP restored by a potion
} Item;

typedef struct {
    int hp, maxHp;
    int mana, maxMana;          // resource for skills
    
    int atk, def;
    int level, xp;
    
    int x, y;                   // position on the map
    
    Item inventory[MAX_ITEMS];
    int itemCount;
    
    Item weapon, armor;         // equipped items
    int hasWeapon, hasArmor;    // 1 = equipped
} Player;

typedef struct {
    char name[32];
    int hp, atk, def, xpReward;
} Enemy;

#endif
