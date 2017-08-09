#include "hitbox.h"
#include <stddef.h>
#include <stdlib.h>

Hitboxes *init_Hitboxes(Hitboxes *self){
    if (self == NULL){
        return NULL;
    }
    
    self->numMovement = 0;
    self->numInteract = 0;
    self->movement = NULL;
    self->interact = NULL;
}

void term_Hitbox(Hitbox *self){
    if (self == NULL){
        return;
    }
    
    free(self->rects);
    free(self->circles);
    
    self->numRect = 0;
    self->numCircle = 0;
    self->rects = NULL;
    self->circles = NULL;
}

void term_Hitboxes(Hitboxes *self){
    if (self == NULL){
        return;
    }
    
    size_t i;
    for (i = 0; i < self->numMovement; i++){
        term_Hitbox(self->movement + i);
    }
    free(self->movement);
    self->movement = NULL;
    
    for (i = 0; i < self->numInteract; i++){
        term_Hitbox(self->interact + i);
    }
    free(self->interact);
    self->interact = NULL;
    
    self->numMovement = 0;
    self->numInteract = 0;
}