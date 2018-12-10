#include "weapon_00000.h"
#include "../src/enemy.h"
#include "../src/graphics.h"
#include "../src/data_reader.h"
#include "../src/player.h"
#include <stdlib.h>
#include <stdio.h>

static const int DAMAGE = 4;

static void doSword(void *self, int delta);
static void collideWithSword(Weapon *self, void *o, int collCode, CollisionType t);
static void drawSword(Entity *self, double shiftX, double shfitY);

Weapon *weapon_create_00000(Weapon *self){
    if (self == NULL){
        self = malloc(sizeof(Weapon));
    }
    init_Weapon(self);
        
    self->e.active = 0;
    self->e.action = &doSword;
    readAnimationIntoEntity((Entity *)self, 2);
    
    self->e.pixelsPerMilli = 50;
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
    self->e.numFrames = 3;
	self->e.orientation = DOWN;
//	void (*collide)(struct Entity*);
	self->e.type = WEAPON;
	
//	self->totalDelta = 0;
//	self->cancelled = 0;
       
    self->collide = &collideWithSword;
    
    //the icon for the menu
    self->icon = init_Entity(malloc(sizeof(Entity)));
    readAnimationIntoEntity(self->icon, 14);
    
    //the icon for the ui
    self->iconSmall = init_Entity(malloc(sizeof(Entity)));
    readAnimationIntoEntity(self->iconSmall, 22);
    
    return self;
}

static void doSword(void *w, int delta){
    Weapon *self = (Weapon*)w;

    //update the animation
    updateAnimation(self->e.animation, delta);
    
    self->e.milliPassed += delta;
    self->e.orientation = _player.e.orientation;
    self->e.x = _player.e.x;
    self->e.y = _player.e.y;
    
    //x and y adjustments would have to be handled specifically for the sprite?  Do they need to be set by the owner?
    if (self->e.orientation == UP){
        setAnimationLoop(self->e.animation, 3, 0);
        self->e.y -= _player.e.h;
    } else if (self->e.orientation == DOWN){
        setAnimationLoop(self->e.animation, 0, 0);
        self->e.x -= _player.e.w;
    } else if (self->e.orientation == LEFT){
        setAnimationLoop(self->e.animation, 1, 0);
        self->e.x -= _player.e.w;
        self->e.y -= _player.e.h;
    } else if (self->e.orientation == RIGHT){
        setAnimationLoop(self->e.animation, 2, 0);
        self->e.y -= _player.e.h;
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

void weapon_collide_00000(Weapon *self, void *o, int collCode, CollisionType t){
    collideWithSword(self, o, collCode, t);
}

