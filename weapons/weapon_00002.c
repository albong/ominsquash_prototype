#include "weapon_00000.h"
#include "../src/enemy.h"
#include "../src/graphics.h"
#include "../src/data_reader.h"
#include <stdio.h>

static const int DAMAGE = 4;

static void createHitBoxes(Weapon *w);
static void doWeapon(void *self, int delta);
static void collideWithSword(Weapon *self, void *o, int collCode, CollisionType t);

Weapon *weapon_create_00002(){
    Weapon *w = init_Weapon(malloc(sizeof(Weapon)));
    
    w->e.active = 1;
    
    w->e.action = &doWeapon;
    w->e.sprite = readSpriteFromFile("data/sprites/00006.sprite", NULL);
    w->e.animation = readSpriteAnimationFromFile("data/animations/00006.animation", NULL);
    
    w->e.pixelsPerMilli = 500;
//    double changeX, changeY;
//    w->e.x = 0;
//    w->e.y = 0;
//    w->e.changeX = 0;
//    w->e.changeY = 0;
//	int w, h;
    w->e.isMoving = 1;
//    w->e.milliPassed = 0;
    w->e.milliPerFrame = 30;
//    w->e.currFrame = 0;
    w->e.numFrames = 2;
	w->e.orientation = DOWN;
//	void (*collide)(struct Entity*);
	w->e.type = WEAPON;
	
//	w->totalDelta = 0;
//	w->cancelled = 0;
       
    createHitBoxes(w);
    w->collide = &collideWithSword;
    
    return w;
}

void createHitBoxes(Weapon *w){
    w->e.currHitBox = 0;
    w->e.hasMoveHitBox = 0;
	w->e.hasInteractHitBox = 1;
	w->e.interactHitBox = malloc(sizeof(HitBox) * 2);
	
	w->e.interactHitBox[0].numRect = 1;
	w->e.interactHitBox[0].rects = malloc(sizeof(CollRect));
	w->e.interactHitBox[0].rects[0].x = 1;
	w->e.interactHitBox[0].rects[0].y = 2;
	w->e.interactHitBox[0].rects[0].w = 6;
	w->e.interactHitBox[0].rects[0].h = 4;
	
	w->e.interactHitBox[1].numRect = 1;
	w->e.interactHitBox[1].rects = malloc(sizeof(CollRect));
	w->e.interactHitBox[1].rects[0].x = 2;
	w->e.interactHitBox[1].rects[0].y = 1;
	w->e.interactHitBox[1].rects[0].w = 4;
	w->e.interactHitBox[1].rects[0].h = 6;
	
}

void doWeapon(void *w, int delta){
    Weapon *self = (Weapon*)w;
    
    //update the animation
    updateAnimation(self->e.animation, delta);
    
    //check if the bullet is out of bounds - should account for sprite size here using hitboxes?
    if (self->e.x < 0 + self->e.w || self->e.x > SCREEN_WIDTH){
	    self->e.active = 0;
	}
	if (self->e.y + self->e.h < 0 || self->e.y  > SCREEN_HEIGHT){
	    self->e.active = 0;
	}
    
    //move the bullet
    self->e.changeX = 0;
    self->e.changeY = 0;
    if (self->e.orientation == UP){
        setAnimationLoop(self->e.animation, 1, 0);
		self->e.changeY = self->e.pixelsPerMilli * delta/1000.0 * -1;
	} else if (self->e.orientation == DOWN){
        setAnimationLoop(self->e.animation, 1, 0);
		self->e.changeY = self->e.pixelsPerMilli * delta/1000.0;
	} else if (self->e.orientation == LEFT){
        setAnimationLoop(self->e.animation, 0, 0);
		self->e.changeX = self->e.pixelsPerMilli * delta/1000.0 * -1;
	} else if (self->e.orientation == RIGHT){
        setAnimationLoop(self->e.animation, 0, 0);
		self->e.changeX = self->e.pixelsPerMilli * delta/1000.0;
	}
}

void collideWithSword(Weapon *self, void *o, int collCode, CollisionType t){
    if (t == ENEMY){
        Enemy *enemy = (Enemy *)o;
        if (enemy->takeDamage(enemy, DAMAGE)){
            addImpactMove(&enemy->e, self->e.x, self->e.y, 20, 1);
            self->e.active = 0;
            self->cancelled = 1;
        }
    }
}

void weapon_collide_00002(Weapon *self, void *o, int collCode, CollisionType t){
    collideWithSword(self, o, collCode, t);
}