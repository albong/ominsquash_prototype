#include "enemy.h"

static Sprite *defaultDeathSprite = NULL;

/////////////////////////////////////////////////
// Loading
/////////////////////////////////////////////////
Enemy *init_Enemy(Enemy *self){
    if (self == NULL){
        return NULL;
    }
    
    init_Entity((Entity *)self);
    self->health = 0;
    self->milliHitstun = 0;
    self->takeDamage = NULL;
    self->collide = NULL;
    self->action = NULL;

    return self;
}

Sprite *getDefaultDeathSprite(){
    if (defaultDeathSprite == NULL){
        defaultDeathSprite = loadAnimatedSprite("gfx/explosion.png", 27);
    }
    return defaultDeathSprite;
}

