#include "weapon_00000.h"
#include "../src/enemy.h"
#include "../src/graphics.h"
#include "../src/data_reader.h"

static const int DAMAGE = 4;

static void doSword(void *self, int delta);
static void collideWithSword(Weapon *self, void *o, int collCode, CollisionType t);

Weapon *weapon_create_00000(){
    Weapon *w = init_Weapon(malloc(sizeof(Weapon)));
        
    w->e.active = 1;
    w->e.action = &doSword;
    readAnimationIntoEntity((Entity *)w, 2);
    
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
       
    w->collide = &collideWithSword;
    
    //the icon for the menu
    w->icon = init_Entity(malloc(sizeof(Entity)));
    readAnimationIntoEntity(w->icon, 14);
    
    return w;
}

static void doSword(void *w, int delta){
    Weapon *self = (Weapon*)w;

    //update the animation
    updateAnimation(self->e.animation, delta);
    
    self->e.milliPassed += delta;
    self->e.orientation = self->owner->orientation;
    self->e.x = self->owner->x;
    self->e.y = self->owner->y;
    
    //x and y adjustments would have to be handled specifically for the sprite?  Do they need to be set by the owner?
    if (self->e.orientation == UP){
        setAnimationLoop(self->e.animation, 3, 0);
        self->e.y -= self->owner->h;
    } else if (self->e.orientation == DOWN){
        setAnimationLoop(self->e.animation, 0, 0);
        self->e.x -= self->owner->w;
    } else if (self->e.orientation == LEFT){
        setAnimationLoop(self->e.animation, 1, 0);
        self->e.x -= self->owner->w;
        self->e.y -= self->owner->h;
    } else if (self->e.orientation == RIGHT){
        setAnimationLoop(self->e.animation, 2, 0);
        self->e.y -= self->owner->h;
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