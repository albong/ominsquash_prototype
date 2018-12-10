#include "weapon_00001.h"
#include "../src/enemy.h"
#include "../src/graphics.h"
#include "../src/data_reader.h"
#include "../src/player.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_PROJECTILES 50

static const int DAMAGE = 0;

static Weapon projectiles[NUM_PROJECTILES];

static void doWeapon(void *self, int delta);
static void collideWithSword(Weapon *self, void *o, int collCode, CollisionType t);
static void createProjectiles();

Weapon *weapon_create_00001(Weapon *self){
    if (self == NULL){
        self = malloc(sizeof(Weapon));
    }
    init_Weapon(self);
    
    self->e.active = 0;
    
    self->e.action = &doWeapon;
    // self->e.sprite = readSpriteFromFile("data/sprites/00002.sprite", NULL);
    // self->e.animation = readAnimationFromFile("data/animations/no_animation.animation", NULL);
    
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
    
    //make the bullets
    createProjectiles();
    
    //make the icon
    self->icon = init_Entity(malloc(sizeof(Entity)));
    readAnimationIntoEntity(self->icon, 15);
    
    //the icon for the ui
    self->iconSmall = init_Entity(malloc(sizeof(Entity)));
    readAnimationIntoEntity(self->iconSmall, 24);
    
    return self;
}

void doWeapon(void *w, int delta){
    Weapon *self = (Weapon*)w;
    
    //loop along until you find an unused bullet, then set that one to draw
    size_t i;
    Weapon *bullet;
    for (i = 0; i < NUM_PROJECTILES; i++){
        if (!projectiles[i].e.active){
            bullet = projectiles + i;
            bullet->e.active = 1;
            bullet->e.x = _player.e.x;
            bullet->e.y = _player.e.y;
            bullet->e.orientation = _player.e.orientation;
            // bullet->owner = self->owner;
            addTempEntityToArea((Entity *)bullet);
            break;
        }
    }
    
    self->cancelled = 1;
}

void createProjectiles(){
    size_t i;
    for (i = 0; i < NUM_PROJECTILES; i++){
        // projectiles[i] = createWeaponById(2);
        projectiles[i].e.active = 0;
    }
}
