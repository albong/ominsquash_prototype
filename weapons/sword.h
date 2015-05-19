#ifndef SWORD_H
#define SWORD_H

#include "../src/Entity.h"

static void loadSwordSprite();
Entity *createSword();
static void doSword(Entity *self, int delta);
static void drawSword(Entity *self, double shiftX, double shiftY);
//Entity *createOctorok(int spriteIndex);
//static void doOctorok(Entity *self, int delta);
//static void updatePosition(Entity *self, int delta);
//static void drawEntity(Entity *self, double shiftX, double shiftY);
//static void updateFrame(Entity *self, int delta);

#endif
