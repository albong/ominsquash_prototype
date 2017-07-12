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
    w->e.sprite = readSpriteFromFile("data/sprites/00002.sprite", NULL);
    // w->e.animation = readAnimationFromFile("data/animations/no_animation.animation", NULL);
    w->e.animation = readAnimationFromFile("data/animations/00002.animation", NULL);
    w->e.hitboxes = *(readHitboxesFromFile("data/hitboxes/00002.hitbox", &(w->e.hitboxes), 0));
    
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
    
    return w;
}

// static void createHitboxes(Weapon *w){
    // w->e.hitboxes.numMovement = 0;
	// w->e.hitboxes.numInteract = 12;
	// w->e.hitboxes.interact = malloc(sizeof(Hitbox) * 12);
	
	// // w->e.hitboxes.interact[0].numRect = 1;
	// // w->e.hitboxes.interact[0].rects = malloc(sizeof(CollRect));
	// // w->e.hitboxes.interact[0].rects[0].x = 1;
	// // w->e.hitboxes.interact[0].rects[0].y = 10;
	// // w->e.hitboxes.interact[0].rects[0].w = 15;
	// // w->e.hitboxes.interact[0].rects[0].h = 4;
	
	// // w->e.hitboxes.interact[1].numRect = 1;
	// // w->e.hitboxes.interact[1].rects = malloc(sizeof(CollRect));
	// // w->e.hitboxes.interact[1].rects[0].x = 4;
	// // w->e.hitboxes.interact[1].rects[0].y = 16;
	// // w->e.hitboxes.interact[1].rects[0].w = 15;
	// // w->e.hitboxes.interact[1].rects[0].h = 15;
	
	// // w->e.hitboxes.interact[2].numRect = 1;
	// // w->e.hitboxes.interact[2].rects = malloc(sizeof(CollRect));
	// // w->e.hitboxes.interact[2].rects[0].x = 25;
	// // w->e.hitboxes.interact[2].rects[0].y = 17;
	// // w->e.hitboxes.interact[2].rects[0].w = 4;
	// // w->e.hitboxes.interact[2].rects[0].h = 15;
	
	
	// // w->e.hitboxes.interact[3].numRect = 1;
	// // w->e.hitboxes.interact[3].rects = malloc(sizeof(CollRect));
	// // w->e.hitboxes.interact[3].rects[0].x = 21;
	// // w->e.hitboxes.interact[3].rects[0].y = 1;
	// // w->e.hitboxes.interact[3].rects[0].w = 4;
	// // w->e.hitboxes.interact[3].rects[0].h = 15;
	
	// // w->e.hitboxes.interact[4].numRect = 1;
	// // w->e.hitboxes.interact[4].rects = malloc(sizeof(CollRect));
	// // w->e.hitboxes.interact[4].rects[0].x = 4;
	// // w->e.hitboxes.interact[4].rects[0].y = 4;
	// // w->e.hitboxes.interact[4].rects[0].w = 15;
	// // w->e.hitboxes.interact[4].rects[0].h = 15;
	
	// // w->e.hitboxes.interact[5].numRect = 1;
	// // w->e.hitboxes.interact[5].rects = malloc(sizeof(CollRect));
	// // w->e.hitboxes.interact[5].rects[0].x = 1;
	// // w->e.hitboxes.interact[5].rects[0].y = 26;
	// // w->e.hitboxes.interact[5].rects[0].w = 15;
	// // w->e.hitboxes.interact[5].rects[0].h = 4;
	
	
    // // w->e.hitboxes.interact[6].numRect = 1;
	// // w->e.hitboxes.interact[6].rects = malloc(sizeof(CollRect));
	// // w->e.hitboxes.interact[6].rects[0].x = 6;
	// // w->e.hitboxes.interact[6].rects[0].y = 1;
	// // w->e.hitboxes.interact[6].rects[0].w = 4;
	// // w->e.hitboxes.interact[6].rects[0].h = 15;
	
	// // w->e.hitboxes.interact[7].numRect = 1;
	// // w->e.hitboxes.interact[7].rects = malloc(sizeof(CollRect));
	// // w->e.hitboxes.interact[7].rects[0].x = 12;
	// // w->e.hitboxes.interact[7].rects[0].y = 4;
	// // w->e.hitboxes.interact[7].rects[0].w = 15;
	// // w->e.hitboxes.interact[7].rects[0].h = 15;
	
	// // w->e.hitboxes.interact[8].numRect = 1;
	// // w->e.hitboxes.interact[8].rects = malloc(sizeof(CollRect));
	// // w->e.hitboxes.interact[8].rects[0].x = 15;
	// // w->e.hitboxes.interact[8].rects[0].y = 26;
	// // w->e.hitboxes.interact[8].rects[0].w = 15;
	// // w->e.hitboxes.interact[8].rects[0].h = 4;
	
	
	// // w->e.hitboxes.interact[9].numRect = 1;
	// // w->e.hitboxes.interact[9].rects = malloc(sizeof(CollRect));
	// // w->e.hitboxes.interact[9].rects[0].x = 15;
	// // w->e.hitboxes.interact[9].rects[0].y = 22;
	// // w->e.hitboxes.interact[9].rects[0].w = 15;
	// // w->e.hitboxes.interact[9].rects[0].h = 4;
	
	// // w->e.hitboxes.interact[10].numRect = 1;
	// // w->e.hitboxes.interact[10].rects = malloc(sizeof(CollRect));
	// // w->e.hitboxes.interact[10].rects[0].x = 12;
	// // w->e.hitboxes.interact[10].rects[0].y = 4;
	// // w->e.hitboxes.interact[10].rects[0].w = 15;
	// // w->e.hitboxes.interact[10].rects[0].h = 15;
	
	// // w->e.hitboxes.interact[11].numRect = 1;
	// // w->e.hitboxes.interact[11].rects = malloc(sizeof(CollRect));
	// // w->e.hitboxes.interact[11].rects[0].x = 1;
	// // w->e.hitboxes.interact[11].rects[0].y = 2;
	// // w->e.hitboxes.interact[11].rects[0].w = 4;
	// // w->e.hitboxes.interact[11].rects[0].h = 15;
// }

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