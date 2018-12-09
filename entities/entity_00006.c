#include "entity_00006.h"
#include "../src/weapon.h"

void entity_collidePlayer_00006(Entity *self, int collisionCode){
    self->active = 0;
    givePlayerWeapon(0);
}
