#ifndef OCTOROK_H
#define OCTOROK_H

#include "../src/Entity.h"
#include "../src/Enemy.h"

Enemy *createOctorok(Sprite *sprite);
static void doOctorok(Enemy *self, int delta);
static void updatePosition(Enemy *self, int delta);
static void drawEntity(Entity * self, double shiftX, double shiftY);
static void updateFrame(Enemy *self, int delta);
#endif
