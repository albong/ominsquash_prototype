#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "hitbox.h"
#include "weapon.h"
#include "entity.h"

// Logic
void doCollisions();
int checkPlayerCollideEntitiesMovement(Entity **entityList, size_t numEntities);

// Collisions
static int rectangleCollide(CollRect r1, CollRect r2);
static int circleCollide(CollCircle c1, CollCircle c2);
static int rectangleCircleCollide(CollRect r, CollCircle c);

#endif
