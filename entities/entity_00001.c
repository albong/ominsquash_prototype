#include "entity_00001.h"
#include <stdio.h>

#include "../src/entity.h"
#include "../src/graphics.h"

void entity_action_00001(void *e, int delta){
    Entity *self = (Entity *)e;
    int currLoop = self->animation->currLoop;
    
    self->animation->milliPassed += delta;
    if (self->animation->milliPassed >= self->animation->loopTotalDuration[currLoop]){
        self->active = 0;
    }
}
