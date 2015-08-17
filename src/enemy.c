#include "enemy.h"

static Sprite *defaultDeathSprite = NULL;

Sprite *getDefaultDeathSprite(){
    if (defaultDeathSprite == NULL){
        defaultDeathSprite = loadAnimatedSprite("gfx/explosion.png", 27);
    }
    return defaultDeathSprite;
}

