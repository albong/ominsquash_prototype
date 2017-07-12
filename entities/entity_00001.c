#include "entity_00001.h"
#include <stdio.h>

#include "../src/entity.h"
#include "../src/graphics.h"

void entity_action_00001(void *e, int delta){
    Entity *self = (Entity *)e;
    
    if (updateAnimation(self->animation, delta) == 2){
        self->active = 0;
    }
}
