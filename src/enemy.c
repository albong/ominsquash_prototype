#include "enemy.h"
#include "player.h"
#include "data_reader.h"

static void defaultDrawEnemy(Entity *self, double shiftX, double shiftY);
static void defaultCollidePlayer(Entity *self, int collCode);
/////////////////////////////////////////////////
// Loading / Unloading
/////////////////////////////////////////////////
Enemy *init_Enemy(Enemy *self){
    if (self == NULL){
        return NULL;
    }
    
    init_Entity((Entity *)self);
    self->health = 0;
    self->milliHitstun = 0;
    self->deathEntity = NULL;
    self->touchDamage = 0;
    self->takeDamage = NULL;
    self->action = NULL;
    
    self->e.collidePlayer = &defaultCollidePlayer;
    self->e.type = ENEMY;
    self->e.draw = &defaultDrawEnemy;

    return self;
}

void free_Enemy(Enemy *self){
    if (self == NULL){
        return;
    }
    
    free_Entity(self->deathEntity);
    self->deathEntity = NULL;
    
    free_Entity((Entity *)self);
}


/////////////////////////////////////////////////
// Default methods
/////////////////////////////////////////////////
void defaultDrawEnemy(Entity *self, double shiftX, double shiftY){
    if (((Enemy *)self)->health > 0 && self->sprite != NULL){
        drawAnimation(self->sprite, self->animation, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY);
    }
}

void defaultCollidePlayer(Entity *self, int collCode){
    if (playerTakeDamage(((Enemy *)self)->touchDamage)){
        addImpactMove(&_player.e, self->x, self->y, 15, 1);
    }
}
