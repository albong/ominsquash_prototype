#include "weapon.h"
#include <stdlib.h>

void initWeaponLists(){
    _player_weapons.num = 0;
    _enemy_weapons.num = 0;
}

void addPlayerWeapon(Entity *e){
    realloc(_player_weapons.weapons, _player_weapons.num + 1);
    _player_weapons.weapons[_player_weapons.num] = e;
    _player_weapons.num += 1;
}
