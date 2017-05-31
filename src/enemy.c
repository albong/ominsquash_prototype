#include "enemy.h"
#include "player.h"
#include "data_reader.h"

static NewSprite *defaultDeathSprite = NULL;
static SpriteAnimation *defaultDeathAnimation = NULL;

static NewSprite *getDefaultDeathSprite();
static SpriteAnimation *getDefaultDeathAnimation();
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
    self->deathSprite = getDefaultDeathSprite();
    self->deathAnimation = getDefaultDeathAnimation();
    self->touchDamage = 0;
    self->takeDamage = NULL;
    self->collidePlayer = &defaultCollidePlayer;
    self->action = NULL;
    
    self->e.draw = &defaultDrawEnemy;

    return self;
}

NewSprite *getDefaultDeathSprite(){
    if (defaultDeathSprite == NULL){
        defaultDeathSprite = readNewSpriteFromFile("data/sprites/00003.sprite", NULL);
    }
    return defaultDeathSprite;
}

SpriteAnimation *getDefaultDeathAnimation(){
    if (defaultDeathAnimation == NULL){
        defaultDeathAnimation = readSpriteAnimationFromFile("data/animations/00003.animation", NULL);
    }
    return defaultDeathAnimation;
}

void defaultDrawEnemy(Entity *self, double shiftX, double shiftY){
    if (((Enemy *)self)->health > 0 && self->sprite != NULL){
        drawAnimation(self->sprite, self->animation, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY);
    } else if (((Enemy *)self)->deathSprite != NULL){
        drawAnimation(((Enemy *)self)->deathSprite, ((Enemy *)self)->deathAnimation, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY);
    }
}

void defaultCollidePlayer(Enemy *self, int collCode){
    if (playerTakeDamage(self->touchDamage)){
        addImpactMove(&_player.e, self->e.x, self->e.y, 15, 1);
    }
}
