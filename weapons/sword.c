#include "sword.h"
#include "../src/graphics.h"

static Sprite *swordSprite;

static void loadSwordSprite(){
    if (swordSprite == NULL){
        swordSprite = loadAnimatedSprite("gfx/swordsprite.png", 31);
    }
}

Weapon *createSword(){
    loadSwordSprite();
    Weapon *w = malloc(sizeof(Weapon));
        
    w->e.active = 1;
    
    w->e.action = &doSword;
    w->e.sprite = swordSprite;
    
    w->e.pixelsPerMilli = 50;
//    double changeX, changeY;
    w->e.x = 0;
    w->e.y = 0;
    w->e.changeX = 0;
    w->e.changeY = 0;
//	int w, h;
    w->e.isMoving = 1;
    w->e.milliPassed = 0;
    w->e.milliPerFrame = 30;
    w->e.currFrame = 0;
    w->e.numFrames = 3;
	w->e.orientation = DOWN;
    w->e.draw = &drawSword;
//	void (*collide)(struct Entity*);
	w->e.type = WEAPON;
	
	w->totalDelta = 0;
       
    return w;
}

static void doSword(void *w, int delta){
    Weapon *self = (Weapon*)w;
    
    self->e.milliPassed += delta;
    self->e.orientation = self->owner->orientation;
    self->e.x = self->owner->x;
    self->e.y = self->owner->y;
    
    int frameNum = self->e.milliPassed / self->e.milliPerFrame;
    if (frameNum >= self->e.numFrames){
        frameNum = self->e.numFrames-1;
        self->e.milliPassed -= delta;
    }
    
    //x and y adjustments would have to be handled specifically for the sprite?
    if (self->e.orientation == UP){
        self->e.currFrame = frameNum + 9;
        self->e.y -= self->owner->h;
    } else if (self->e.orientation == DOWN){
        self->e.currFrame = frameNum + 0;
        self->e.x -= self->owner->w;
    } else if (self->e.orientation == LEFT){
        self->e.currFrame = frameNum + 3;
        self->e.x -= self->owner->w;
        self->e.y -= self->owner->h;
    } else if (self->e.orientation == RIGHT){
        self->e.currFrame = frameNum + 6;
        self->e.y -= self->owner->h;
    }
}

static void drawSword(void *w, double shiftX, double shiftY){
    Weapon *self = (Weapon *)w;

    drawAnimatedSprite(self->e.sprite, self->e.currFrame, self->e.x + 0.5 + shiftX, self->e.y + 0.5 + shiftY);
}
