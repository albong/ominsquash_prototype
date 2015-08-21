#include "octorok.h"
#include "../src/constants.h"
#include <stdlib.h>

static int totalDelta = 0;
static const hitstunMilli = 1000;

Enemy *createOctorok(Sprite *sprite){
    Enemy *enemy = malloc(sizeof(Enemy));
    
    enemy->e.active = 1;
    
    enemy->action = &doOctorok;
    enemy->e.sprite = sprite;
    
    enemy->e.pixelsPerMilli = 50;
//    double changeX, changeY;
    enemy->e.x = 0;
    enemy->e.y = 0;
    enemy->e.changeX = 0;
    enemy->e.changeY = 0;
//	int w, h;
    enemy->e.isMoving = 1;
    enemy->e.milliPassed = 0;
    enemy->e.milliPerFrame = 200;
    enemy->e.numFrames = 2;
	enemy->e.orientation = DOWN;
    enemy->e.draw = &drawEntity;
//	void (*collide)(struct Entity*);
	enemy->e.type = ENEMY;

    enemy->e.currHitBox = 0;
    
    enemy->e.hasMoveHitBox = 1;
    enemy->e.moveHitBox = malloc(sizeof(HitBox) * 1);
    enemy->e.moveHitBox[0].numCircle = 0;
    enemy->e.moveHitBox[0].numRect = 1;
    enemy->e.moveHitBox[0].rects = malloc(sizeof(CollRect) * 1);
    enemy->e.moveHitBox[0].rects[0].x = 1;
    enemy->e.moveHitBox[0].rects[0].y = 5;
    enemy->e.moveHitBox[0].rects[0].w = 16;
    enemy->e.moveHitBox[0].rects[0].h = 8;

    enemy->e.hasInteractHitBox = 1;
    enemy->e.interactHitBox = malloc(sizeof(HitBox) * 1);
    enemy->e.interactHitBox[0].numCircle = 0;
    enemy->e.interactHitBox[0].numRect = 1;
    enemy->e.interactHitBox[0].rects = malloc(sizeof(CollRect) * 1);
    enemy->e.interactHitBox[0].rects[0].x = 1;
    enemy->e.interactHitBox[0].rects[0].y = 2;
    enemy->e.interactHitBox[0].rects[0].w = 16;
    enemy->e.interactHitBox[0].rects[0].h = 11;
    
    enemy->health = 8;
    enemy->milliHitstun = 0;
    enemy->takeDamage = &damageOctorok;
    
    return enemy;
}

static void doOctorok(Enemy *self, int delta){
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
    
    self->milliHitstun = (self->milliHitstun - delta < 0) ? 0 : self->milliHitstun - delta;
}

static void updatePosition(Enemy *self, int delta){
    if (self->health <= 0){
        return;
    }
    
    self->e.changeX = 0;
    self->e.changeY = 0;
    if (self->e.orientation == UP){
		self->e.changeY = self->e.pixelsPerMilli * delta/1000.0 * -1;
	} else if (self->e.orientation == DOWN){
		self->e.changeY = self->e.pixelsPerMilli * delta/1000.0;
	} else if (self->e.orientation == LEFT){
		self->e.changeX = self->e.pixelsPerMilli * delta/1000.0 * -1;
	} else if (self->e.orientation == RIGHT){
		self->e.changeX = self->e.pixelsPerMilli * delta/1000.0;
	}
	
	applyExternalMoves(&self->e, delta);
	
	//prevent from drifting off the screen
	if (self->e.x + self->e.changeX < 0 || self->e.x + self->e.sprite->width + self->e.changeX > SCREEN_WIDTH){
	    self->e.changeX = 0;
	}
	if (self->e.y + self->e.changeY < 0 || self->e.y + self->e.sprite->image->h + self->e.changeY > SCREEN_HEIGHT){
	    self->e.changeY = 0;
	}
}

static void drawEntity(Entity *self, double shiftX, double shiftY){
    int frame;
    int invert = (((Enemy *)self)->milliHitstun / HITSTUN_FLASH_MILLI) % 2;
    if (((Enemy *)self)->health > 0){
        if (self->isMoving){
            frame = ((self->milliPassed / self->milliPerFrame) + 1) % self->numFrames;
            switch (self->orientation){
                case UP:
                    drawInvertedAnimatedSprite(self->sprite, 4 + frame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY, invert);
                    break;
                case DOWN:
                    drawInvertedAnimatedSprite(self->sprite, 2 + frame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY, invert);
                    break;
                case LEFT:
                    drawInvertedAnimatedSprite(self->sprite, 0 + frame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY, invert);
                    break;
                case RIGHT:
                    drawInvertedAnimatedSprite(self->sprite, 6 + frame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY, invert);
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
    } else {
        frame = self->milliPassed / MILLI_PER_DEFAULT_DEATH_FRAME;
        if (frame < NUM_FRAMES_DEFAULT_DEATH){
            drawInvertedAnimatedSprite(getDefaultDeathSprite(), 0 + frame, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY, invert);
        } else {
            self->active = 0;
        }
    }
}

static void updateFrame(Enemy *self, int delta){
    if (self->health > 0){
        if (self->e.isMoving){
            self->e.milliPassed += delta;
            self->e.milliPassed %= (self->e.milliPerFrame * self->e.numFrames);
        } else {
            self->e.milliPassed = 0;
        }
    } else {
        self->e.milliPassed += delta;
    }
}

/*
    Perhaps the best way to handle this would actually be to have default methods and some sort of generic initializer for enemies, and there would be some sort of id table with constants
    I see it like this - on creation of an enemy, it checks if a static id number has been set (default value of -1).  If it hasn't then it registers itself to get an id number.
    Using this id number it sets any constants it needs, which are stored in an array statically hidden in the enemy.c file.  The enemy struct now also must hold this id number - make it a
    parameter to the function that instantiates an empty enemy?  When takeDamage, or whatever, is called, if the default version is being used, then we use the id number to find the correct
    constants and do the function.
    Another thing to add here is that this part is probably always going to run, so we might want to add a "constant" function pointer for anything special to call after this has been called
 */
static int damageOctorok(Enemy *self, int damage){
    if (self->milliHitstun == 0){
        self->health -= damage;
        if (self->health <= 0){
            self->e.milliPassed = 0;
        }
        self->milliHitstun = hitstunMilli;
        return 1;
    } else {
        return 0;
    }
}
