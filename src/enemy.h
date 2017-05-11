#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "graphics.h"

#define MILLI_PER_DEFAULT_DEATH_FRAME 500
#define NUM_FRAMES_DEFAULT_DEATH 7

typedef struct Enemy {
    Entity e;
    int health;
    int milliHitstun;
    Sprite *enemySprite;
    Sprite *deathSprite;
    NewSprite *nDeathSprite;
    SpriteAnimation *deathAnimation;
    int touchDamage;
    int (*takeDamage)(struct Enemy *self, int amount);
    void (*collidePlayer)(struct Enemy *self, int collCode);
    void (*action)(struct Enemy *self, int delta);
} Enemy;

// Loading
Enemy *init_Enemy(Enemy *self);
Sprite *getDefaultDeathSprite();

#endif
