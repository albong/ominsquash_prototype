#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "graphics.h"

#define MILLI_PER_DEFAULT_DEATH_FRAME 500
#define NUM_FRAMES_DEFAULT_DEATH 7

typedef struct Enemy {
    Entity e;
    int health;
//    void (*takeDamage)(struct Enemy *self, int amount, double xFrom, double yFrom);
    void (*takeDamage)(struct Enemy *self, int amount);
    void (*collide)(struct Enemy *self, void *o, int collCode, CollisionType type);
    void (*action)(struct Enemy *self, int delta);
} Enemy;

Sprite *getDefaultDeathSprite();

#endif
