#include "hitbox.h"
#include <stddef.h>

Hitboxes *init_Hitboxes(Hitboxes *self){
    if (self == NULL){
        return NULL;
    }
    
    self->numMovement = 0;
    self->numInteract = 0;
    self->movement = NULL;
    self->interact = NULL;
}