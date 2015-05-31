#ifndef OCTOROK_H
#define OCTOROK_H

#include "../src/Entity.h"

Entity *createOctorok(Sprite *sprite);
static void doOctorok(void *e, int delta);
static void updatePosition(Entity *self, int delta);
static void drawEntity(void * e, double shiftX, double shiftY);
static void updateFrame(Entity *self, int delta);
#endif
