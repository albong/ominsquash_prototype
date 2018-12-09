#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "hitbox.h"
#include "weapon.h"
#include "entity.h"

// Logic
void doCollisions();

// Collisions
int rectangleCollide(CollRect r1, CollRect r2);
int circleCollide(CollCircle c1, CollCircle c2);
int rectangleCircleCollide(CollRect r, CollCircle c);

#endif
