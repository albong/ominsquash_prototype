#include "skull.h"

#include "../src/hitbox.h"

static void myInteract(Entity *self);

Entity *createSkull(Sprite *sprite){
    Entity *self = init_Entity(malloc(sizeof(Entity)));
    
    self->sprite = sprite;
    self->numFrames = 1;
    
    self->hasInteractHitBox = 1;
    self->interactHitBox = malloc(sizeof(HitBox) * 1);
    self->interactHitBox[0].numCircle = 0;
    self->interactHitBox[0].circles = NULL;
    self->interactHitBox[0].numRect = 1;
    self->interactHitBox[0].rects = malloc(sizeof(CollRect) * 1);
    self->interactHitBox[0].rects[0].x = 1;
    self->interactHitBox[0].rects[0].y = 1;
    self->interactHitBox[0].rects[0].w = 14;
    self->interactHitBox[0].rects[0].h = 14;
    self->interactable = 1;
    self->interact = &myInteract;
    
    self->active = 1;
    
    return self;
}

void myInteract(Entity *self){
    printf("I sleep now!\n");
}
