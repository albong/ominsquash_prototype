#ifndef WEAPON_TABLES_H
#define WEAPON_TABLES_H

#include "../src/weapon.h"
#include "../src/entity.h"

//these correspond to the pointers used by the Weapon struct, and
//one for creation since weapons aren't finalized yet, to ease development
typedef Weapon *(*weapon_create_ptr_t)(Weapon *);
typedef void (*weapon_collide_ptr_t)(Weapon *, void *, int, CollisionType);

//table size is filled for outside viewing
void fillWeaponTables(weapon_create_ptr_t **createTable, weapon_collide_ptr_t **collideTable, size_t *tableSize);

#endif
