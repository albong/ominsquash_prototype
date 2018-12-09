#include "stair.h"
#include "entity.h"
#include "area.h"

static void defaultCollidePlayer(Entity *selfE, int collisionCode);

Stair *init_Stair(Stair *self){
    if (self == NULL){
        return self;
    }
    
    self->sameArea = 0;
    self->toArea = 0;
    self->toRoom = 0;
    self->toX = 0;
    self->toY = 0;
    
    init_Entity((Entity *)self);
    self->e.collidePlayer = &defaultCollidePlayer;
}

void free_Stair(Stair *self){
    if (self == NULL){
        return;
    }
    
    term_Entity((Entity *)self);
    
    self->sameArea = 0;
    self->toArea = 0;
    self->toRoom = 0;
    self->toX = 0;
    self->toY = 0;
    
    free(self);
}

static void defaultCollidePlayer(Entity *selfE, int collisionCode){
    setChangingRoomFromStair((Stair *)selfE);
}
