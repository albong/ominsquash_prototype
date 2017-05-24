#ifndef ENEMY_00000_H
#define ENEMY_00000_H

#include "../src/entity.h"
#include "../src/enemy.h"

//void enemy_draw_00000(Entity *self, double shiftX, double shiftY);
int enemy_takeDamage_00000(Enemy *self, int damage);
void enemy_action_00000(Enemy *self, int delta);

#endif
