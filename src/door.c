#include "door.h"
#include "hitbox.h"

static Door *init_Door(Door *self);
static void drawDoor(Entity *self, double shiftX, double shiftY);

Door *createDoor(Sprite *sprite, Orientation direction, double x, double y){
    Door *result = init_Door(malloc(sizeof(Door)));
    
    result->e.orientation = direction;
    result->e.x = x;
    result->e.y = y;
    
    result->e.hasInteractHitBox = 1;
    result->e.interactHitBox = malloc(sizeof(HitBox));
    result->e.interactHitBox->numCircle = 0;
    result->e.interactHitBox->numRect = 1;
    result->e.interactHitBox->rects = malloc(sizeof(CollRect));
    
    result->e.sprite = sprite;
    result->e.numFrames = 3;
    if (direction == UP){
        result->e.currFrame = 0;
    } else if (direction == DOWN){
        result->e.currFrame = 3;
    } else if (direction == LEFT){
        result->e.currFrame = 6;
    } else {//if (direction == RIGHT){
        result->e.currFrame = 9;
    }
    
    return result;
}

Door *init_Door(Door *self){
    if (self == NULL){
        return NULL;
    }
    
    init_Entity((Entity *)self);
    self->e.type = DOOR;
    self->isOpen = 0;
    self->changingState = 0;
    self->isLocked = 0;

    return self;
}

void setDoorOpen(Door *self, int open){
    if (self == NULL){
        return;
    }
    
    self->isOpen = open;
}

void setDoorLocked(Door *self, int locked){
    if (self == NULL){
        return;
    }
    
    self->isLocked = locked;
}
