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
    e->x = 0;
    e->y = 0;
    e->changeX = 0;
    e->changeY = 0;
//	int w, h;
    e->isMoving = 1;
    e->milliPassed = 0;
    e->milliPerFrame = 200;
    e->numFrames = 2;
	e->orientation = DOWN;
    e->draw = &drawEntity;
//	void (*collide)(struct Entity*);
	e->type = ENEMY;
//	HitBox *moveHitBox;
//	HitBox *interactHitBox;
    
    return e;
}

static void doOctorok(Entity *self, int delta){
    totalDelta += delta;
    
    if (totalDelta >= 1500){
        if (rand() % 2){
            totalDelta = 0;
            int orientation = rand() % 4;
//            if (orientation == 0){
//                self->orientation = UP;
//            } else if (orientation == 1){
//                self->orientation = DOWN;
//            } else if (orientation == 2){
//                self->orientation = LEFT;
//            } else if (orientation == 3){
//                self->orientation = RIGHT;
//            }
        } else {
            totalDelta -= 750;
        }
    }
    updateFrame(self, delta);
    updatePosition(self, delta);
    move(self);
}

static void updatePosition(Entity *e, int delta){
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
	if (e->x + e->changeX < 0 || e->x + e->sprite->width + e->changeX > SCREEN_WIDTH){
	    e->changeX = 0;
	}
	if (e->y + e->changeY < 0 || e->y + e->sprite->image->h + e->changeY > SCREEN_HEIGHT){
	    e->changeY = 0;
	}
}

static void move(Entity *e){
    e->x += e->changeX;
    e->y += e->changeY;
    e->changeX = 0;
    e->changeY = 0;
}

static void drawEntity(Entity *self, double shiftX, double shiftY){
    if (self->isMoving){
        int frame = ((self->milliPassed / self->milliPerFrame) + 1) % self->numFrames;
        switch (self->orientation){
            case UP:
                drawAnimatedSprite(self->sprite, 4 + frame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY);
                break;
            case DOWN:
                drawAnimatedSprite(self->sprite, 2 + frame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY);
                break;
            case LEFT:
                drawAnimatedSprite(self->sprite, 0 + frame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY);
                break;
            case RIGHT:
                drawAnimatedSprite(self->sprite, 6 + frame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY);
                break;
            default:
                break;
        }
    } else {
//        switch (self->orientation){
//            case UP:
//                drawAnimatedSprite(self->sprite, 4, self->x + 0.5, self->y + 0.5);
//                break;
//            case DOWN:
//                drawAnimatedSprite(self->sprite, 2, self->x + 0.5, self->y + 0.5);
//                break;
//            case LEFT:
//                drawAnimatedSprite(self->sprite, 0, self->x + 0.5, self->y + 0.5);
//                break;
//            case RIGHT:
//                drawAnimatedSprite(self->sprite, 6, self->x + 0.5, self->y + 0.5);
//                break;
//            default:
//                break;
//        }
    }
}

static void updateFrame(Entity *self, int delta){
    if (self->isMoving){
        self->milliPassed += delta;
        self->milliPassed %= (self->milliPerFrame * self->numFrames);
    } else {
        self->milliPassed = 0;
    }
}
