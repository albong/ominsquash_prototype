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

void collideWithWall(CollRect wall, Entity *e, CollRect r, int collCode, int isX){
    double newChangeX = 0;
    double newChangeY = 0;

//    if (collCode & 1){
//        newChangeX = wall.x - r.x - r.w;
//    }
//    if (collCode & 2){
//        newChangeX = wall.x + wall.w - r.x;
//    }
//    if (collCode & 4){
//        newChangeY = wall.y - r.y - r.h;
//    }
//    if (collCode & 8){
//        newChangeY = wall.y + wall.h - r.y;
//    }

    if (isX){
        if (collCode & 1){
            newChangeX = wall.x - r.x - r.w;
            e->changeX = newChangeX;
        }
        if (collCode & 2){
            newChangeX = wall.x + wall.w - r.x;
            e->changeX = newChangeX;
        }
//        if (collCode & 4){
//            newChangeY = wall.y - r.y - r.h;
//        }
//        if (collCode & 8){
//            newChangeY = wall.y + wall.h - r.y;
//        }
    } else {
        if (collCode & 4){
            newChangeY = wall.y - r.y - r.h;
            e->changeY = newChangeY;
        }
        if (collCode & 8){
            newChangeY = wall.y + wall.h - r.y;
            e->changeY = newChangeY;
        }
//        if (collCode & 1){
//            newChangeY = wall.y - r.y - r.h;
//        }
//        if (collCode & 2){
//            newChangeY = wall.y + wall.h - r.y;
//        }
    }
    
//    printf("%f, %f - %f, %f\n", e->changeX, e->changeY, newChangeX, newChangeY);
//    newChangeX = 0;
//    newChangeY = 0;
//    e->changeX = newChangeX;
//    e->changeY = newChangeY;
//    if (square(newChangeX) <= square(e->changeX) && square(newChangeY) <= square(e->changeY)
//        && (newChangeX < 0) == (e->changeX) && (newChangeY < 0) == (e->changeY) ){
//        e->changeX = newChangeX;
//        e->changeY = newChangeY;
//    }

//    if (newChangeX && isX){
//    if (newChangeX){
//        e->changeX = newChangeX;
//    }
////    if (newChangeY && !isX){
//    if (newChangeY){
//        e->changeY = newChangeY;
//    }
}

void drawStandardEntity(){
//    drawImage(self->sprite, self->x, self->y);
}

void drawAnimatedEntity(){
//    drawImage(animation[self->animID].frame[self->currentFrame], self->x, self->y);
}
