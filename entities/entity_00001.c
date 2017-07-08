#include "entity_00001.h"
#include <stdio.h>

#include "../src/entity.h"
#include "../src/graphics.h"

void entity_action_00001(void *e, int delta){
    Entity *self = (Entity *)e;
    updateAnimation(self->animation, delta);
    if (self->animation->currFrame == -1){
        self->active = 0;
    }
}
