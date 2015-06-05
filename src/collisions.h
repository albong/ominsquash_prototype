#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "hitbox.h"
#include "weapon.h"

void doCollisions();
static void doWallCollisions();
static void doEnemyCollisions();
static void doWeaponCollisions();
static void enemiesCollideWithWeapon(Weapon *w);

static int rectangleCollide(CollRect r1, CollRect r2);
static int circleCollide(CollCircle c1, CollCircle c2);
static int rectangleCircleCollide(CollRect r, CollCircle c);

#endif
