#include "door.h"
#include "hitbox.h"

static Door *init_Door(Door *self);
static void doDoor(void *self, int delta);
static void drawDoor(Entity *self, double shiftX, double shiftY);

Door *createDoor(Orientation direction, double x, double y){
    Door *result = init_Door(malloc(sizeof(Door)));
    
    result->e.active = 1;
    
    result->e.orientation = direction;
    result->e.x = x;
    result->e.y = y;
    
    result->e.hasMoveHitBox = 1;
    result->e.moveHitBox = malloc(sizeof(HitBox) * 1);
    result->e.moveHitBox[0].numCircle = 0;
    result->e.moveHitBox[0].numRect = 1;
    result->e.moveHitBox[0].rects = malloc(sizeof(CollRect) * 1);
    result->e.moveHitBox[0].rects[0].x = 0;
    result->e.moveHitBox[0].rects[0].y = 0;
    // result->e.moveHitBox[0].rects[0].w = sprite->width;
    // result->e.moveHitBox[0].rects[0].h = sprite->height;
    
    result->e.hasInteractHitBox = 1;
    result->e.interactHitBox = malloc(sizeof(HitBox) * 1);
    result->e.interactHitBox[0].numCircle = 0;
    result->e.interactHitBox[0].numRect = 1;
    result->e.interactHitBox[0].rects = malloc(sizeof(CollRect) * 1);
    // result->e.interactHitBox[0].rects[0].x = -1 * sprite->width;
    // result->e.interactHitBox[0].rects[0].y = -1 * sprite->width;
    // result->e.interactHitBox[0].rects[0].w = sprite->width * 3;
    // result->e.interactHitBox[0].rects[0].h = sprite->width * 3;
    
    result->e.currHitBox = 0;
    
    result->e.numFrames = 3;
    result->e.milliPerFrame = 75;
    if (direction == UP){
        result->e.currFrame = 0;
    } else if (direction == DOWN){
        result->e.currFrame = 3;
    } else if (direction == LEFT){
        result->e.currFrame = 6;
    } else {//if (direction == RIGHT){
        result->e.currFrame = 9;
    }
    
    result->e.action = &doDoor;
    
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

void doDoor(void *self, int delta){
    Door *selfp = (Door *)self;
    if (!selfp->e.active || !selfp->changingState){
        return;
    }
    
    //this trick works because there are the same number of frames for each door orientation
    int direction;
    if (selfp->e.orientation == UP){
        direction = 0;
    } else if (selfp->e.orientation == DOWN){
        direction = 1;
    } else if (selfp->e.orientation == LEFT){
        direction = 2;
    } else {//if (selfp->e.orientation == RIGHT){
        direction = 3;
    }
    
    //increment the frame up or down
    selfp->e.milliPassed += delta;
    int frame = selfp->e.milliPassed / selfp->e.milliPerFrame;
    if (selfp->isOpen){
        selfp->e.currFrame = (direction * selfp->e.numFrames) + (selfp->e.numFrames - frame - 1);
    } else {
        selfp->e.currFrame = (direction * selfp->e.numFrames) + frame;
    }
    
    //if we've run out of frames then stop, and set state as changed
    if (frame >= selfp->e.numFrames - 1){
        selfp->changingState = 0;
        selfp->isOpen = !selfp->isOpen;
        selfp->e.milliPassed = 0;
    }
}

void setDoorOpening(Door *self, int open){
    if (self == NULL){
        return;
    }
    
    if (!((self->isOpen && open) || (!self->isOpen && !open))){
        self->changingState = 1;
    }
}

void setDoorOpen(Door *self, int open){
    if (self == NULL){
        return;
    }
    
    //this trick works because there are the same number of frames for each door orientation
    int direction;
    if (self->e.orientation == UP){
        direction = 0;
    } else if (self->e.orientation == DOWN){
        direction = 1;
    } else if (self->e.orientation == LEFT){
        direction = 2;
    } else {//if (self->e.orientation == RIGHT){
        direction = 3;
    }
    
    self->isOpen = open;
    self->changingState = 0;
    self->e.currFrame = (direction * self->e.numFrames);
    if (open){
        self->e.currFrame += (self->e.numFrames - 1);
    }
}

void setDoorLocked(Door *self, int locked){
    if (self == NULL){
        return;
    }
    
    self->isLocked = locked;
}
