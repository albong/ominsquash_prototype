#include "entity.h"

#define square(x) (x*x)

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

