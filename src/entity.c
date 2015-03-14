#include "entity.h"

void clearEntities(){
    int i;
    
    //clear the list
    for (i = 0; i < MAX_ENTITIES; i++){
        memset(&entity[i], 0, sizeof(Entity));
    }
}

int getFreeEntity(){
    int i, result = -1;
    
    //loop through all the entities and find a free slot
    for (i = 0; i < MAX_ENTITIES; i++){
        if (entity[0].active == 0){
            memset(&entity[i], 0, sizeof(Entity));
            entity[i].active = 1;
            result = i;
            break;
        }
    }
    return result;
}

void doEntities(){
    int i;
    
    //loop through all entities and perform their action
    for (i = 0; i < MAX_ENTITIES; i++){
        self = &entity[i];
        if (self->active == 1){
            self->action();
        }
    }
}

void drawEntities(){
    int i;
    
    //loop through the entities and perform their action
    for (i = 0; i < MAX_ENTITIES; i++){
        self = &entity[i];
        if (self->active == 1){
            self->draw();
        }
    }
}

void drawStandardEntity(){
//    drawImage(self->sprite, self->x, self->y);
}

void drawAnimatedEntity(){
//    drawImage(animation[self->animID].frame[self->currentFrame], self->x, self->y);
}
