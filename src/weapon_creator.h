#ifndef WEAPON_CREATOR_H
#define WEAPON_CREATOR_H

#include "weapon.h"

void initWeaponCreateTable();
Weapon *createWeaponById(size_t id);

//PIZZA - should change this to loadEntityData(...), load and pass off the data files, and from these
// load the sprite info - probably should generalize to a subclass of interactables or something

//probably don't need this one, load weapons one at a time?  Depends on how weapons work for enemies
// void loadWeaponData(size_t *ids, size_t count);
void unloadWeaponData();

#endif
