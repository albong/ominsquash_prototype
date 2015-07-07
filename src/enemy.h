#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

typedef struct Enemy {
    Entity e;
    int health;
    void (*takeDamage)(struct Enemy *self, int amount);
    void (*collide)(struct Enemy *self, void *o, int collCode, CollisionType type);
    void (*action)(struct Enemy *self, int delta);
} Enemy;

#endif
