#include "octorok.h"
#include "../src/constants.h"
#include <stdlib.h>

static int totalDelta = 0;

Entity *createOctorok(int spriteIndex){
    Entity *e = malloc(sizeof(Entity));
    
    e->active = 1;
    
    e->action = &doOctorok;
    e->sprite = getSprite(spriteIndex);
    
    e->pixelsPerMilli = 50;
//    double changeX, changeY;
//	int w, h;
//	int isMoving;
//	int milliPerFrame, milliPassed;
//	int numFrames;
	e->orientation = DOWN;
//	void (*draw)(void);
//	void (*collide)(struct Entity*);
	e->type = ENEMY;
//	HitBox *moveHitBox;
//	HitBox *interactHitBox;
    
    return e;
}

static void doOctorok(int delta, Entity *e){
    totalDelta += delta;
    
    if (totalDelta >= 1500){
        if (rand() % 2){
            totalDelta = 0;
            int orientation = rand() % 4;
            if (orientation == 0){
                e->orientation = UP;
            } else if (orientation == 1){
                e->orientation = DOWN;
            } else if (orientation == 2){
                e->orientation = LEFT;
            } else if (orientation == 3){
                e->orientation = RIGHT;
            }
        } else {
            totalDelta -= 750;
        }
    }
    updatePosition(delta, e);
    move(e);
}

static void updatePosition(int delta, Entity *e){
    e->changeX = 0;
    e->changeY = 0;
    if (e->orientation == UP){
		e->changeY = e->pixelsPerMilli * delta/1000.0 * -1;

	} else if (e->orientation == DOWN){
		e->changeY = e->pixelsPerMilli * delta/1000.0;

	} else if (e->orientation == LEFT){
		e->changeX = e->pixelsPerMilli * delta/1000.0 * -1;

	} else if (e->orientation == RIGHT){
		e->changeX = e->pixelsPerMilli * delta/1000.0;
	}
	
	//prevent from drifting off the screen
	if (e->x + e->changeX < 0 || e->x + e->changeX > SCREEN_WIDTH){
	    e->changeX = 0;
	}
	if (e->y + e->changeY < 0 || e->y + e->changeY > SCREEN_HEIGHT){
	    e->changeY = 0;
	}
}

static void move(Entity *e){
    e->x += e->changeX;
    e->y += e->changeY;
    e->changeX = 0;
    e->changeY = 0;
}
