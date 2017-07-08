#include "weapon_00001.h"
#include "../src/enemy.h"
#include "../src/graphics.h"
#include "../src/data_reader.h"
#include "../src/weapon_creator.h"
#include <stdio.h>

#define NUM_PROJECTILES 50

static const int DAMAGE = 0;

static Weapon *projectiles[NUM_PROJECTILES];

static void doWeapon(void *self, int delta);
static void collideWithSword(Weapon *self, void *o, int collCode, CollisionType t);
static void createProjectiles();

Weapon *weapon_create_00001(){
    Weapon *w = init_Weapon(malloc(sizeof(Weapon)));
        
    w->e.active = 1;
    
    w->e.action = &doWeapon;
    // w->e.sprite = readSpriteFromFile("data/sprites/00002.sprite", NULL);
    // w->e.animation = readSpriteAnimationFromFile("data/animations/no_animation.animation", NULL);
    
    w->e.pixelsPerMilli = 50;
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
    w->e.numFrames = 3;
	w->e.orientation = DOWN;
//	void (*collide)(struct Entity*);
	w->e.type = WEAPON;
	
//	w->totalDelta = 0;
//	w->cancelled = 0;
    
    createProjectiles();
    
    return w;
}

void doWeapon(void *w, int delta){
    Weapon *self = (Weapon*)w;
    
    //loop along until you find an unused bullet, then set that one to draw
    size_t i;
    Weapon *bullet;
    for (i = 0; i < NUM_PROJECTILES; i++){
        if (!projectiles[i]->e.active){
            bullet = projectiles[i];
            bullet->e.active = 1;
            bullet->e.x = self->owner->x;
            bullet->e.y = self->owner->y;
            bullet->e.orientation = self->owner->orientation;
            bullet->owner = self->owner;
            addTempEntityToArea((Entity *)bullet);
            break;
        }
    }
    
    self->cancelled = 1;
}

void createProjectiles(){
    size_t i;
    for (i = 0; i < NUM_PROJECTILES; i++){
        projectiles[i] = createWeaponById(2);
        projectiles[i]->e.active = 0;
    }
}