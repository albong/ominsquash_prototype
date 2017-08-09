#include "weapon.h"
#include <stdlib.h>

/////////////////////////////////////////////////
// Loading / Unloading
/////////////////////////////////////////////////
Weapon *init_Weapon(Weapon *self){
    if (self == NULL){
        return NULL;
    }

    init_Entity((Entity *)self);
    self->owner = NULL;
    self->totalDelta = 0;
    self->cancelled = 0;
    self->collide = NULL;
    
    return self;
}

void initWeaponLists(){
    _player_weapons.num = 0;
    _enemy_weapons.num = 0;
    
    _player_weapons.weapons = malloc(0);
    _enemy_weapons.weapons = malloc(0);
}

void termWeaponLists(){
    size_t i;
    
    for (i = 0; i < _player_weapons.num; i++){
        free_Entity((Entity *)_player_weapons.weapons[i]);
    }
    free(_player_weapons.weapons);
    
    for (i = 0; i < _enemy_weapons.num; i++){
        free_Entity((Entity *)_enemy_weapons.weapons[i]);
    }
    free(_enemy_weapons.weapons);
}


/////////////////////////////////////////////////
// Access
/////////////////////////////////////////////////
void addPlayerWeapon(Weapon *w){
    _player_weapons.weapons = (Weapon **)realloc(_player_weapons.weapons, (_player_weapons.num + 1) * sizeof(Weapon *));
    _player_weapons.weapons[_player_weapons.num] = w;
    _player_weapons.num++;
}
