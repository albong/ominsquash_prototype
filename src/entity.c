#include "entity.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define square(x) ((x)*(x))

static void defaultActionEntity(void *e, int delta);
static void defaultDrawEntity(Entity *self, double shiftX, double shiftY);

/////////////////////////////////////////////////
// Loading / Unloading
/////////////////////////////////////////////////
Entity *init_Entity(Entity *self){
    if (self == NULL){
        return NULL;
    }
    
    self->active = 0;
    self->pixelsPerMilli = 0;
    self->x = 0;
    self->y = 0;
    self->startX = 0;
    self->startY = 0;
    self->changeX = 0;
    self->changeY = 0;
    self->numExternalMove = 0;
    self->externalMove = NULL;
	self->w = 0;
    self->h = 0;
	self->isMoving = 0;
	self->milliPerFrame = 0;
    self->milliPassed = 0;
	self->currFrame = 0;
	self->numFrames = 0;
	self->invertSprite = 0;
    self->sprite = NULL;
    self->animation = NULL;
	self->orientation = UP;
	// self->action = NULL;
    self->construct = NULL;
    self->destruct = NULL;
	self->action = &defaultActionEntity;
	self->draw = &defaultDrawEntity;
	self->collidePlayer = NULL;
    self->type = NONE;
    init_Hitboxes(&(self->hitboxes));
    self->interactable = 0;
	self->interact = NULL;
    return self;
}

void term_Entity(Entity *self){
    if (self == NULL){
        return;
    }
    
    free(self->externalMove);
    self->externalMove = NULL;
    self->numExternalMove = 0;
    
    free_Sprite(self->sprite);
    free_Animation(self->animation);
    
    term_Hitboxes(&(self->hitboxes));
    
    if (self->destruct != NULL){
        self->destruct(self);
    }
}

void free_Entity(Entity *self){
    if (self == NULL){
        return;
    }
    
    term_Entity(self);
    free(self);
}


/////////////////////////////////////////////////
// Logic
/////////////////////////////////////////////////
void moveEntity(Entity *e){
    e->x += e->changeX;
    e->y += e->changeY;
    e->changeX = 0;
    e->changeY = 0;
}

void collideWithWallX(CollRect wall, Entity *e, CollRect r, int collCode){
    if (collCode & 1){
        e->changeX = wall.x - r.x - r.w;
    }
    if (collCode & 2){
        e->changeX = wall.x + wall.w - r.x;
    }
}

void collideWithWallY(CollRect wall, Entity *e, CollRect r, int collCode){
    if (collCode & 4){
        e->changeY = wall.y - r.y - r.h;
    }
    if (collCode & 8){
        e->changeY = wall.y + wall.h - r.y;
    }
}

void defaultExternalMove(Entity *self, double x, double y, double magnitude){
    //compute the components of the change vector
    double delY = sqrt(square(magnitude) / (square(x/y) + 1));
    double delX = (x/y) * y;
    
    //set the sign
    delX = (x < 0 && delX > 0) ? -delX : delX;
    delY = (y < 0 && delY > 0) ? -delY : delY;
    
}

void addExternalMove(Entity *self, double x, double y, double magnitude, double velocity){
    //compute the components of the change vector
    double delY = sqrt(square(magnitude) / (square(x/y) + 1));
    double delX = (x/y) * y;
    delX = (x < 0 && delX > 0) ? -delX : delX;
    delY = (y < 0 && delY > 0) ? -delY : delY;
    
    //add to the array
    self->numExternalMove++;
    self->externalMove = realloc(self->externalMove, sizeof(EntityExtMove) * self->numExternalMove);
    self->externalMove[self->numExternalMove-1].delX = delX;
    self->externalMove[self->numExternalMove-1].delY = delY;
    self->externalMove[self->numExternalMove-1].distance = magnitude;
    self->externalMove[self->numExternalMove-1].velocity = velocity;
}

void addImpactMove(Entity *self, double xFrom, double yFrom, double magnitude, double velocity){
    addExternalMove(self, self->x - xFrom, self->y - yFrom, magnitude, velocity);
}

void applyExternalMoves(Entity *self, int delta){
    size_t i;
    double delX, delY, distance, scale;
    int velocity;
    for (i = 0; i < self->numExternalMove; i++){
        delX = self->externalMove[i].delX;
        delY = self->externalMove[i].delY;
        distance = self->externalMove[i].distance;
        velocity = self->externalMove[i].velocity;
        scale = sqrt(square(velocity * delta) / (square(delX) + square(delY)));
        self->changeX += scale * delX;
        self->changeY += scale * delY;
        distance -= velocity * delta;
        if (distance <= 0){
            memmove(self->externalMove + i, self->externalMove + i + 1, sizeof(EntityExtMove) * (self->numExternalMove - i - 1));
            self->numExternalMove--;
            self->externalMove = realloc(self->externalMove, sizeof(EntityExtMove) * self->numExternalMove);
            i--;
        } else {
            self->externalMove[i].distance = distance;
        }
    }
}


/////////////////////////////////////////////////
// Default methods
/////////////////////////////////////////////////
void defaultActionEntity(void *e, int delta){
    updateAnimation(((Entity *)e)->animation, delta);
}

void defaultDrawEntity(Entity *self, double shiftX, double shiftY){
    if (self != NULL && self->sprite != NULL && self->animation != NULL){
        drawAnimation(self->sprite, self->animation, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY);
    }
}
