#include "weapon_00000.h"
#include "../src/enemy.h"
#include "../src/graphics.h"
#include "../src/data_reader.h"
#include <stdio.h>
#include <stdlib.h>

static const int DAMAGE = 4;

static void createHitboxes(Weapon *w);
static void doWeapon(void *self, int delta);
static void collideWithSword(Weapon *self, void *o, int collCode, CollisionType t);

Weapon *weapon_create_00002(Weapon *self){
    if (self == NULL){
        self = malloc(sizeof(Weapon));
    }
    init_Weapon(self);
    
    self->e.active = 1;
    
    self->e.action = &doWeapon;
    self->e.sprite = readSpriteFromFile("data/sprites/00006.sprite", NULL);
    self->e.animation = readAnimationFromFile("data/animations/00006.animation", NULL);
    
    self->e.pixelsPerMilli = 500;
//    double changeX, changeY;
//    self->e.x = 0;
//    self->e.y = 0;
//    self->e.changeX = 0;
//    self->e.changeY = 0;
//	int w, h;
    self->e.isMoving = 1;
//    self->e.milliPassed = 0;
    self->e.milliPerFrame = 30;
//    self->e.currFrame = 0;
    self->e.numFrames = 2;
	self->e.orientation = DOWN;
//	void (*collide)(struct Entity*);
	self->e.type = WEAPON;
	
//	self->totalDelta = 0;
//	self->cancelled = 0;
       
    createHitboxes(self);
    self->collide = &collideWithSword;
    
    return self;
}

void createHitboxes(Weapon *w){
    w->e.hitboxes.numMovement = 0;
	w->e.hitboxes.numInteract = 1;
	w->e.hitboxes.interact = malloc(sizeof(Hitbox) * 2);
	
	w->e.hitboxes.interact[0].numRect = 1;
	w->e.hitboxes.interact[0].rects = malloc(sizeof(CollRect));
	w->e.hitboxes.interact[0].rects[0].x = 1;
	w->e.hitboxes.interact[0].rects[0].y = 2;
	w->e.hitboxes.interact[0].rects[0].w = 6;
	w->e.hitboxes.interact[0].rects[0].h = 4;
	
	w->e.hitboxes.interact[1].numRect = 1;
	w->e.hitboxes.interact[1].rects = malloc(sizeof(CollRect));
	w->e.hitboxes.interact[1].rects[0].x = 2;
	w->e.hitboxes.interact[1].rects[0].y = 1;
	w->e.hitboxes.interact[1].rects[0].w = 4;
	w->e.hitboxes.interact[1].rects[0].h = 6;
	
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