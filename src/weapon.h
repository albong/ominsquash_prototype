#ifndef WEAPON_H
#define WEAPON_H

#include "entity.h"

//should have two load methods - one for loading all of the player's weapons, and the other for loading the weapons for an area
//in actuality we probably can add player weapons one at a time since it won't happen too often, so extending the loaded weapon list is not so costly
typedef struct WeaponList {
    int num;
    Entity **weapons; //array
} WeaponList;

WeaponList _player_weapons;
WeaponList _enemy_weapons;

void initWeaponLists();
void addPlayerWeapon(Entity *e);

#endif
