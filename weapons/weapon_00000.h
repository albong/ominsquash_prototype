#ifndef SWORD_H
#define SWORD_H

#include "../src/Entity.h"
#include "../src/Weapon.h"

//static void loadSwordSprite();
Weapon *weapon_create_00000();
//static void createHitBoxes(Weapon *w);
//static void doSword(void *self, int delta);
//static void collideWithSword(Weapon *self, void *o, int collCode, CollisionType t);
void weapon_collide_00000(Weapon *self, void *o, int collCode, CollisionType t);

#endif