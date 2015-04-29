#ifndef OCTOROK_H
#define OCTOROK_H

#include "../src/Entity.h"

Entity *createOctorok(int spriteIndex);
static void doOctorok(Entity *self, int delta);
static void updatePosition(Entity *self, int delta);
static void move(Entity *self);
static void drawEntity(Entity *self, double shiftX, double shiftY);
static void updateFrame(Entity *self, int delta);
#endif
