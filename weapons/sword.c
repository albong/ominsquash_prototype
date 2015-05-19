#include "sword.h"
#include "../src/graphics.h"

static Sprite *swordSprite;

static void loadSwordSprite(){
    if (swordSprite == NULL){
        swordSprite = getSprite(loadAnimatedSprite("gfx/swordsprite.png", 31));
    }
}

Entity *createSword(){
    loadSwordSprite();
    Entity *e = malloc(sizeof(Entity));
    
    e->active = 1;
    
    e->action = &doSword;
    e->sprite = swordSprite;
    
    e->pixelsPerMilli = 50;
//    double changeX, changeY;
    e->x = 0;
    e->y = 0;
    e->changeX = 0;
    e->changeY = 0;
//	int w, h;
    e->isMoving = 1;
    e->milliPassed = 0;
    e->milliPerFrame = 200;
    e->numFrames = 2;
	e->orientation = DOWN;
    e->draw = &drawSword;
//	void (*collide)(struct Entity*);
	e->type = WEAPON;
    
    return e;
}

static void doSword(Entity *self, int delta){
    
}

static void drawSword(Entity *self, double shiftX, double shiftY){
    
}
