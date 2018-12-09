#include "weapon.h"
#include "../weapons/weapon_tables.h"
#include "entity.h"
#include "logging.h"
#include <stdlib.h>


/////////////////////////////////////////////////
// Loading / Unloading
/////////////////////////////////////////////////
Weapon *init_Weapon(Weapon *self){
    if (self == NULL){
        LOG_WAR("Received null pointer initializing weapon");
        return NULL;
    }
    init_Entity((Entity *)self);

    self->totalDelta = 0;
    self->cancelled = 0;
    self->collide = NULL;
    self->icon = NULL;
    self->iconSmall = NULL;
    self->playerHas = 0;
    
    LOG_INF("Weapon at %p initialized", self);
    return self;
}

void term_Weapon(Weapon *self){
    if (self == NULL){
        LOG_WAR("Received null pointer terminating weapon");
        return;
    }

    self->totalDelta = 0;
    self->cancelled = 0;
    self->collide = NULL;
    free_Entity(self->icon);
    free_Entity(self->iconSmall);
    self->playerHas = 0;
    
    LOG_INF("Weapon at %p terminated", self);
}

void free_Weapon(Weapon *self){
    if (self == NULL){
        LOG_WAR("Received null pointer freeing weapon");
        return;
    }
    
    term_Weapon(self);
    free(self);
    LOG_INF("Weapon at %p freed", self);
}

void initWeaponLists(){
    //load in the function pointers
    weapon_create_ptr_t *weaponCreateTable; //array
    weapon_collide_ptr_t *weaponCollideTable; //array
    fillWeaponTables(&weaponCreateTable, &weaponCollideTable, &_num_player_weapons);
    
    //create each of the weapons - we'll only ever need one copy of each
    size_t i;
    _player_weapons = malloc(sizeof(Weapon) * _num_player_weapons);
    for (i = 0; i < _num_player_weapons; i++){
        init_Weapon(_player_weapons+i); //the create will reinit, but not everything will have a create so this is good
        if (weaponCreateTable[i] != NULL){
            weaponCreateTable[i](_player_weapons+i);
        } else {
            //do nothing
        }
    }
    
    //free the pointer tables
    free(weaponCreateTable);
    free(weaponCollideTable);
    
    LOG_INF("Weapon tables filled");
}

void termWeaponLists(){
    //create each of the weapons - we'll only ever need one copy of each
    size_t i;
    for (i = 0; i < _num_player_weapons; i++){
        //PIZZA - run destruct method here
        term_Weapon(_player_weapons+i);
    }
    free(_player_weapons);
    _num_player_weapons = 0;
    
    LOG_INF("Weapon tables freed");
}


/////////////////////////////////////////////////
// Access
/////////////////////////////////////////////////
void givePlayerWeapon(int id){
    if (0 <= id && id < _num_player_weapons){
        _player_weapons[id].playerHas = 1;
    } else {
        LOG_ERR("No weapon with ID %d", id);
    }
}
