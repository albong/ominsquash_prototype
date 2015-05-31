#ifndef WEAPON_H
#define WEAPON_H

#include "entity.h"

//should have two load methods - one for loading all of the player's weapons, and the other for loading the weapons for an area
//in actuality we probably can add player weapons one at a time since it won't happen too often, so extending the loaded weapon list is not so costly

//do we need two sets of weapons - one for the player (owner is player) and one for enemies (owner is enemy)?

typedef struct Weapon {
    Entity e;
    Entity *owner;
    int totalDelta;
} Weapon;

typedef struct WeaponList {
    int num;
    Weapon **weapons; //array
} WeaponList;

WeaponList _player_weapons;
WeaponList _enemy_weapons;

void initWeaponLists();
void addPlayerWeapon(Weapon *w);

#endif
