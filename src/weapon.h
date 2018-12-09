#ifndef WEAPON_H
#define WEAPON_H

#include "entity.h"

///////
// We should have damage types - not in a rock paper scissors way a la pokemon, more a way for enemies to indicate their vulnerability to types
// makes it easy for weapons to determine if they do damage to an enemy
///////

// Structs
typedef struct Weapon {
    Entity e;
    int totalDelta;
    int cancelled;
    void (*collide)(struct Weapon *self, void *o, int collCode, CollisionType type);
    Entity *icon; //for the menu
    Entity *iconSmall; //for the in-game ui
    int playerHas;
} Weapon;

// Globals
size_t _num_player_weapons;
Weapon *_player_weapons; //array

// Loading/Unloading
Weapon *init_Weapon(Weapon *self);
void term_Weapon(Weapon *self);
void free_Weapon(Weapon *self);
void initWeaponLists();
void termWeaponLists();

// Access
void givePlayerWeapon(int id);

#endif
