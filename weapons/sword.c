#include "sword.h"
#include "../src/enemy.h"
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
       
    createHitBoxes(w);
    w->collide = &collideWithSword;
    
    return w;
}

static void createHitBoxes(Weapon *w){
    w->e.currHitBox = 0;
	w->e.hasInteractHitBox = 1;
	w->e.interactHitBox = malloc(sizeof(HitBox) * 12);
	
	w->e.interactHitBox[0].numRect = 1;
	w->e.interactHitBox[0].rects = malloc(sizeof(CollRect));
	w->e.interactHitBox[0].rects[0].x = 1;
	w->e.interactHitBox[0].rects[0].y = 10;
	w->e.interactHitBox[0].rects[0].w = 15;
	w->e.interactHitBox[0].rects[0].h = 4;
	
	w->e.interactHitBox[1].numRect = 1;
	w->e.interactHitBox[1].rects = malloc(sizeof(CollRect));
	w->e.interactHitBox[1].rects[0].x = 4;
	w->e.interactHitBox[1].rects[0].y = 16;
	w->e.interactHitBox[1].rects[0].w = 15;
	w->e.interactHitBox[1].rects[0].h = 15;
	
	w->e.interactHitBox[2].numRect = 1;
	w->e.interactHitBox[2].rects = malloc(sizeof(CollRect));
	w->e.interactHitBox[2].rects[0].x = 25;
	w->e.interactHitBox[2].rects[0].y = 17;
	w->e.interactHitBox[2].rects[0].w = 4;
	w->e.interactHitBox[2].rects[0].h = 15;
	
	
	w->e.interactHitBox[3].numRect = 1;
	w->e.interactHitBox[3].rects = malloc(sizeof(CollRect));
	w->e.interactHitBox[3].rects[0].x = 21;
	w->e.interactHitBox[3].rects[0].y = 1;
	w->e.interactHitBox[3].rects[0].w = 4;
	w->e.interactHitBox[3].rects[0].h = 15;
	
	w->e.interactHitBox[4].numRect = 1;
	w->e.interactHitBox[4].rects = malloc(sizeof(CollRect));
	w->e.interactHitBox[4].rects[0].x = 4;
	w->e.interactHitBox[4].rects[0].y = 4;
	w->e.interactHitBox[4].rects[0].w = 15;
	w->e.interactHitBox[4].rects[0].h = 15;
	
	w->e.interactHitBox[5].numRect = 1;
	w->e.interactHitBox[5].rects = malloc(sizeof(CollRect));
	w->e.interactHitBox[5].rects[0].x = 1;
	w->e.interactHitBox[5].rects[0].y = 26;
	w->e.interactHitBox[5].rects[0].w = 15;
	w->e.interactHitBox[5].rects[0].h = 4;
	
	
    w->e.interactHitBox[6].numRect = 1;
	w->e.interactHitBox[6].rects = malloc(sizeof(CollRect));
	w->e.interactHitBox[6].rects[0].x = 6;
	w->e.interactHitBox[6].rects[0].y = 1;
	w->e.interactHitBox[6].rects[0].w = 4;
	w->e.interactHitBox[6].rects[0].h = 15;
	
	w->e.interactHitBox[7].numRect = 1;
	w->e.interactHitBox[7].rects = malloc(sizeof(CollRect));
	w->e.interactHitBox[7].rects[0].x = 12;
	w->e.interactHitBox[7].rects[0].y = 4;
	w->e.interactHitBox[7].rects[0].w = 15;
	w->e.interactHitBox[7].rects[0].h = 15;
	
	w->e.interactHitBox[8].numRect = 1;
	w->e.interactHitBox[8].rects = malloc(sizeof(CollRect));
	w->e.interactHitBox[8].rects[0].x = 15;
	w->e.interactHitBox[8].rects[0].y = 26;
	w->e.interactHitBox[8].rects[0].w = 15;
	w->e.interactHitBox[8].rects[0].h = 4;
	
	
	w->e.interactHitBox[9].numRect = 1;
	w->e.interactHitBox[9].rects = malloc(sizeof(CollRect));
	w->e.interactHitBox[9].rects[0].x = 15;
	w->e.interactHitBox[9].rects[0].y = 22;
	w->e.interactHitBox[9].rects[0].w = 15;
	w->e.interactHitBox[9].rects[0].h = 4;
	
	w->e.interactHitBox[10].numRect = 1;
	w->e.interactHitBox[10].rects = malloc(sizeof(CollRect));
	w->e.interactHitBox[10].rects[0].x = 12;
	w->e.interactHitBox[10].rects[0].y = 4;
	w->e.interactHitBox[10].rects[0].w = 15;
	w->e.interactHitBox[10].rects[0].h = 15;
	
	w->e.interactHitBox[11].numRect = 1;
	w->e.interactHitBox[11].rects = malloc(sizeof(CollRect));
	w->e.interactHitBox[11].rects[0].x = 1;
	w->e.interactHitBox[11].rects[0].y = 2;
	w->e.interactHitBox[11].rects[0].w = 4;
	w->e.interactHitBox[11].rects[0].h = 15;
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
    
    //x and y adjustments would have to be handled specifically for the sprite?  Do they need to be set by the owner?
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
    self->e.currHitBox = self->e.currFrame;
}

static void drawSword(Entity *self, double shiftX, double shiftY){
    drawAnimatedSprite(self->sprite, self->currFrame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY);
}

static void collideWithSword(Weapon *self, void *o, int collCode, CollisionType t){
    if (t == ENEMY){
        printf("Hit something\n");
        Enemy *enemy = (Enemy *)o;
        enemy->health -= 1;
    }
}
