#include "enemy.h"
#include "player.h"

static Sprite *defaultDeathSprite = NULL;
static void defaultDrawEnemy(Entity *self, double shiftX, double shiftY);
static void defaultCollidePlayer(Enemy *self, int collCode);
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
    self->collidePlayer = &defaultCollidePlayer;
    self->action = NULL;
    
    self->e.draw = &defaultDrawEnemy;

    return self;
}

Sprite *getDefaultDeathSprite(){
    if (defaultDeathSprite == NULL){
        defaultDeathSprite = loadAnimatedSprite("gfx/explosion.png", 27);
    }
    return defaultDeathSprite;
}

void defaultDrawEnemy(Entity *self, double shiftX, double shiftY){
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

void defaultCollidePlayer(Enemy *self, int collCode){
    if (playerTakeDamage(self->touchDamage)){
        addImpactMove(&_player.e, self->e.x, self->e.y, 15, 1);
    }
}
