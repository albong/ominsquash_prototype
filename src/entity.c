#include "entity.h"

#define square(x) (x*x)

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

void drawEntities(){
    int i;
    
    //loop through the entities and call their draw
    for (i = 0; i < MAX_ENTITIES; i++){
        self = &entity[i];
        if (self->active == 1){
            self->draw();
        }
    }
}

void moveEntities(){
    int i;
    
    //loop through the entities and update their positions
    for (i = 0; i < MAX_ENTITIES; i++){
        self = &entity[i];
        if (self->active == 1){
            self->x += self->changeX;
            self->y += self->changeY;
            self->changeX = 0;
            self->changeY = 0;
        }
    }
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

void drawStandardEntity(){
//    drawImage(self->sprite, self->x, self->y);
}

void drawAnimatedEntity(){
//    drawImage(animation[self->animID].frame[self->currentFrame], self->x, self->y);
}
