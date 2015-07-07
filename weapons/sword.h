#ifndef SWORD_H
#define SWORD_H

#include "../src/Entity.h"
#include "../src/Weapon.h"

static void loadSwordSprite();
Weapon *createSword();
static void createHitBoxes(Weapon *w);
static void doSword(void *self, int delta);
static void drawSword(Entity *self, double shiftX, double shiftY);
static void collideWithSword(Weapon *self, void *o, int collCode, CollisionType t);

//Entity *createOctorok(int spriteIndex);
//static void doOctorok(Entity *self, int delta);
//static void updatePosition(Entity *self, int delta);
//static void drawEntity(Entity *self, double shiftX, double shiftY);
//static void updateFrame(Entity *self, int delta);

#endif
