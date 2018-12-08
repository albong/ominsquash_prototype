#ifndef WEAPON_00002_H
#define WEAPON_00002_H

#include "../src/entity.h"
#include "../src/weapon.h"

//static void loadSwordSprite();
Weapon *weapon_create_00002(Weapon *self);
//static void createHitboxes(Weapon *w);
//static void doSword(void *self, int delta);
//static void collideWithSword(Weapon *self, void *o, int collCode, CollisionType t);
void weapon_collide_00002(Weapon *self, void *o, int collCode, CollisionType t);

#endif
