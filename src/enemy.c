#include "enemy.h"

static Sprite *defaultDeathSprite = NULL;
static void drawEnemy(Entity *self, double shiftX, double shiftY);

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
    self->enemySprite = NULL;
    self->deathSprite = getDefaultDeathSprite();
    self->touchDamage = 0;
    self->takeDamage = NULL;
    self->collide = NULL;
    self->action = NULL;
    
    self->e.draw = &drawEnemy;

    return self;
}

Sprite *getDefaultDeathSprite(){
    if (defaultDeathSprite == NULL){
        defaultDeathSprite = loadAnimatedSprite("gfx/explosion.png", 27);
    }
    return defaultDeathSprite;
}

static void drawEnemy(Entity *self, double shiftX, double shiftY){
    if (((Enemy *)self)->health > 0 && self->sprite != NULL){
        switch (self->orientation){
            case UP:
                drawInvertedAnimatedSprite(self->sprite, self->currFrame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY, self->invertSprite);
                break;
            case DOWN:
                drawInvertedAnimatedSprite(self->sprite, self->currFrame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY, self->invertSprite);
                break;
            case LEFT:
                drawInvertedAnimatedSprite(self->sprite, self->currFrame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY, self->invertSprite);
                break;
            case RIGHT:
                drawInvertedAnimatedSprite(self->sprite, self->currFrame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY, self->invertSprite);
                break;
            default:
                break;
        }
    } else if (((Enemy *)self)->deathSprite != NULL){
        if (self->currFrame < NUM_FRAMES_DEFAULT_DEATH){
            drawAnimatedSprite(((Enemy *)self)->deathSprite, self->currFrame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY);
        } else {
            self->active = 0;
        }
    }
}

