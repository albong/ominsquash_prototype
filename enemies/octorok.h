#ifndef ENEMY_OCTOROK_H
#define ENEMY_OCTOROK_H

//#include "../src/entity.h"
#include "../src/enemy.h"
#include "../src/graphics.h"

/*
    I see no reason why Octorok couldn't be its own subclass of enemy if necessary.
    It would just be defined in the header here and then would get cast correctly in the return
    of createOctorok.
*/

Enemy *createOctorok(Sprite *sprite);
//static void doOctorok(Enemy *self, int delta);
//static void updatePosition(Enemy *self, int delta);
//static void drawEntity(Entity * self, double shiftX, double shiftY);
//static void updateFrame(Enemy *self, int delta);
//static int damageOctorok(Enemy *self, int damage);
#endif
