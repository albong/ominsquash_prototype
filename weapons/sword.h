#ifndef SWORD_H
#define SWORD_H

#include "../src/Weapon.h"

static void loadSwordSprite();
Weapon *createSword();
static void doSword(Weapon *self, int delta);
static void drawSword(Weapon *self, double shiftX, double shiftY);
//Entity *createOctorok(int spriteIndex);
//static void doOctorok(Entity *self, int delta);
//static void updatePosition(Entity *self, int delta);
//static void drawEntity(Entity *self, double shiftX, double shiftY);
//static void updateFrame(Entity *self, int delta);

#endif
