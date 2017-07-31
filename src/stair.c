#include "stair.h"
#include "entity.h"

Stair *init_Stair(Stair *self){
    if (self == NULL){
        return self;
    }
    
    init_Entity((Entity *)self);
    self->sameArea = 0;
    self->toArea = 0;
    self->toRoom = 0;
    self->toTileX = 0;
    self->toTileY = 0;
}