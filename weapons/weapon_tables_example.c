#include "weapon_tables.h"


#include "weapon_00000.h"

void fillWeaponTables(weapon_create_ptr_t **createTable, weapon_collide_ptr_t **collideTable, size_t *tableSize){
    *tableSize = 1;

    weapon_create_ptr_t *createT = malloc(sizeof(weapon_create_ptr_t) * 1);
    weapon_collide_ptr_t *collideT = malloc(sizeof(weapon_collide_ptr_t) * 1);

    createT[00000] = &weapon_create_00000;
    collideT[00000] = &weapon_collide_00000;

    *createTable = createT;
    *collideTable = collideT;
}
