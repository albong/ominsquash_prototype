#include "enemy_00000.h"
#include "../src/constants.h"
#include "../src/entity.h"
#include "../src/graphics.h"
#include "../src/hitbox.h"
#include "../src/data_reader.h"
#include <stdlib.h>

static int totalDelta = 0;
static const hitstunMilli = 1000;
static const DEFAULT_HEALTH = 8;

static void updatePosition(Enemy *self, int delta);
static void updateFrame(Enemy *self, int delta);

// Enemy *createOctorok(Sprite *sprite){
    // return readEnemyFromFile("data/enemies/00000.enemy", NULL);
    // // return createOctorokOLD(sprite);
// }

// void enemy_draw_00000(Entity *self, double shiftX, double shiftY){
    // drawAnimation(self->sprite, self->animation, self->x + 0.5 + shiftX, self->y + 0.5 + shiftY);
// }

void enemy_action_00000(Enemy *self, int delta){
    totalDelta += delta;
    self->e.animation->milliPassed += delta;
    
    if (totalDelta >= 1500){
        if (rand() % 2){
            totalDelta = 0;
            int orientation = rand() % 4;
           if (orientation == 0){
               self->e.orientation = UP;
           } else if (orientation == 1){
               self->e.orientation = DOWN;
           } else if (orientation == 2){
               self->e.orientation = LEFT;
           } else if (orientation == 3){
               self->e.orientation = RIGHT;
           }
        } else {
            totalDelta -= 750;
        }
    }
    // updateFrame(self, delta);
    updatePosition(self, delta);
    
    //update animation loop
    //remember, Entity has an "isMoving" field for this exact thing
    switch (self->e.orientation){
        case UP:
            self->e.animation->currLoop = 0;
            break;
        case DOWN:
            self->e.animation->currLoop = 5;
            break;
        case LEFT:
            self->e.animation->currLoop = 6;
            break;
        case RIGHT:
            self->e.animation->currLoop = 3;
            break;
        default:
            break;
    }
    
    self->milliHitstun = (self->milliHitstun - delta < 0) ? 0 : self->milliHitstun - delta;
}

void updatePosition(Enemy *self, int delta){
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
	if (self->e.x + self->e.changeX < 0 || self->e.x + self->e.w + self->e.changeX > SCREEN_WIDTH){
	    self->e.changeX = 0;
	}
	if (self->e.y + self->e.changeY < 0 || self->e.y + self->e.h + self->e.changeY > SCREEN_HEIGHT){
	    self->e.changeY = 0;
	}
}

void updateFrame(Enemy *self, int delta){
    int frame;
    int invert = (self->milliHitstun / HITSTUN_FLASH_MILLI) % 2;
    
    if (self->health > 0){
        if (self->e.isMoving){
            self->e.milliPassed += delta;
            self->e.milliPassed %= (self->e.milliPerFrame * self->e.numFrames);
        } else {
            self->e.milliPassed = 0;
        }
        
        frame = ((self->e.milliPassed / self->e.milliPerFrame) + 1) % self->e.numFrames;
        switch (self->e.orientation){
            case UP:
                self->e.currFrame = 4 + frame;
                self->e.invertSprite = invert;
                break;
            case DOWN:
                self->e.currFrame = 2 + frame;
                self->e.invertSprite = invert;
                break;
            case LEFT:
                self->e.currFrame = 0 + frame;
                self->e.invertSprite = invert;
                break;
            case RIGHT:
                self->e.currFrame = 6 + frame;
                self->e.invertSprite = invert;
                break;
            default:
                break;
        }
    } else {
        self->e.milliPassed += delta;
        frame = self->e.milliPassed / MILLI_PER_DEFAULT_DEATH_FRAME;
        self->e.invertSprite = 0;
        
        if (frame < NUM_FRAMES_DEFAULT_DEATH){
            self->e.currFrame = 0 + frame;
        } else {
            self->e.active = 0;
        }
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
int enemy_takeDamage_00000(Enemy *self, int damage){
    if (self->milliHitstun == 0 && self->health > 0){
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
