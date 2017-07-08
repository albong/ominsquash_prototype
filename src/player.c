#include "player.h"
#include "graphics.h"
#include "constants.h"
#include "input.h"
#include "weapon.h"
#include "weapon_creator.h"
#include "inventory.h"
#include "data_reader.h"

#include <stdio.h>

static int roomTransition = 0;
static int transitionDirection = -1;
static unsigned totalDelta = 0;
static const hitstunMilli = 1000;

/////////////////////////////////////////////////
// Loading
/////////////////////////////////////////////////
void initPlayer(){
    _player.e.sprite = readSpriteFromFile("data/sprites/00004.sprite", NULL);
    _player.e.animation = readSpriteAnimationFromFile("data/animations/00004.animation", NULL);
    _player.e.orientation = DOWN;    
    
    _player.e.x = SCREEN_WIDTH / 2;
    _player.e.y = SCREEN_HEIGHT / 2;
    
    _player.e.h = 18;
    _player.e.w = 15;
    
    _player.e.milliPerFrame = 200;
    _player.e.milliPassed = 0;
    _player.e.pixelsPerMilli = 70;
    _player.e.numFrames = 2;
    
    _player.e.active = 1;
    
    _player.e.type = PLAYER;
    
    _player.e.currHitBox = 0;
    
    //movement hitbox
    _player.e.hasMoveHitBox = 1;
    _player.e.moveHitBox = malloc(sizeof(HitBox) * 1);
    _player.e.moveHitBox[0].numCircle = 0;
    _player.e.moveHitBox[0].numRect = 1;
    _player.e.moveHitBox[0].rects = malloc(sizeof(CollRect) * 1);
    _player.e.moveHitBox[0].rects[0].x = 4;
    _player.e.moveHitBox[0].rects[0].y = 5;
    _player.e.moveHitBox[0].rects[0].w = _player.e.w-8;
    _player.e.moveHitBox[0].rects[0].h = _player.e.h - 5-2;
    
    //interaction hitbox
    _player.e.hasInteractHitBox = 1;
    _player.e.interactHitBox = malloc(sizeof(HitBox) * 1);
    _player.e.interactHitBox[0].numCircle = 0;
    _player.e.interactHitBox[0].numRect = 1;
    _player.e.interactHitBox[0].rects = malloc(sizeof(CollRect) * 1);
    _player.e.interactHitBox[0].rects[0].x = -3;
    _player.e.interactHitBox[0].rects[0].y = -3;
    _player.e.interactHitBox[0].rects[0].w = 22;
    _player.e.interactHitBox[0].rects[0].h = 24;
        
    _player.health = 12;
    _player.shields = 14;
    _player.milliHitstun = 0;
    
    //weapon
    // Weapon *sword = createSword();
    Weapon *sword = createWeaponById(0);
    sword->owner = &_player.e;
    addPlayerWeapon(sword);
    addWeaponToInventory(sword);
    _player.equippedAInd = 0;
    
    Weapon *gun = createWeaponById(1);
    gun->owner = &_player.e;
    addPlayerWeapon(gun);
    addWeaponToInventory(gun);
    _player.equippedBInd = 1;
}


/////////////////////////////////////////////////
// Logic
/////////////////////////////////////////////////
void doPlayer(int delta){
    totalDelta += delta;
    if (_player.e.active == 1){
		//if movement key is pressed
        if (_input.down || _input.up || _input.left || _input.right){
		    _player.e.isMoving = 1;
		} else {
		    _player.e.isMoving = 0;
		}
		
		//weapon key pressed
		if (_player.equippedAInd >= 0){
    		if (_input.a && !_player_weapons.weapons[_player.equippedAInd]->cancelled){
    		    _player_weapons.weapons[_player.equippedAInd]->e.active = 1;
    		    _player_weapons.weapons[_player.equippedAInd]->e.action(_player_weapons.weapons[_player.equippedAInd], delta);
    		} else if (!_input.a){
    		    _player_weapons.weapons[_player.equippedAInd]->e.active = 0;
    		    _player_weapons.weapons[_player.equippedAInd]->e.milliPassed = 0;
    		    _player_weapons.weapons[_player.equippedAInd]->cancelled = 0;
    		}
        }
		if (_player.equippedBInd >= 0){
    		if (_input.b && !_player_weapons.weapons[_player.equippedBInd]->cancelled){
    		    _player_weapons.weapons[_player.equippedBInd]->e.active = 1;
    		    _player_weapons.weapons[_player.equippedBInd]->e.action(_player_weapons.weapons[_player.equippedBInd], delta);
    		} else if (!_input.b){
    		    _player_weapons.weapons[_player.equippedBInd]->e.active = 0;
    		    _player_weapons.weapons[_player.equippedBInd]->e.milliPassed = 0;
    		    _player_weapons.weapons[_player.equippedBInd]->cancelled = 0;
    		}
        }
        
        //hitstun decrement
        _player.milliHitstun = (_player.milliHitstun - delta < 0) ? 0 : _player.milliHitstun - delta;
        _player.e.invertSprite = (_player.milliHitstun / HITSTUN_FLASH_MILLI) % 2;

        //move player and etc
        updatePlayerPosition(delta);
		updatePlayerOrientation();
		updatePlayerFrame(delta);
    } else if (roomTransition){
        double transPercent = totalDelta / MILLI_PER_TRANSITION;
        switch (transitionDirection){
           case ROOM_LEFT:
               _player.e.x = transPercent * (SCREEN_WIDTH - _player.e.w);
               break;
            case ROOM_RIGHT:
               _player.e.x = SCREEN_WIDTH - _player.e.w - (transPercent * (SCREEN_WIDTH - _player.e.w));
               break;
            case ROOM_UP:
               _player.e.y = transPercent * (SCREEN_HEIGHT - _player.e.h);
               break;
            case ROOM_DOWN:
               _player.e.y = SCREEN_HEIGHT - _player.e.h - (transPercent * (SCREEN_HEIGHT - _player.e.h));
               break;
           default:
                break;
        }
    }
}

void setPlayerTransitioning(int direction){
    roomTransition = 1;
    transitionDirection = direction;
    _player.e.active = 0;
    totalDelta = 0;
}

void stopPlayerTransitioning(){
    switch(transitionDirection){
        case ROOM_LEFT:
            _player.e.x = SCREEN_WIDTH - _player.e.w;
            break;
        case ROOM_RIGHT:
            _player.e.x = 0;
            break;
        case ROOM_UP:
            _player.e.y = SCREEN_HEIGHT - _player.e.h;
            break;
        case ROOM_DOWN:
            _player.e.y = 0;
            break;
    }

    roomTransition = 0;
    transitionDirection = -1;
    _player.e.active = 1;
    totalDelta = 0;
}

int playerTakeDamage(int amount){
    if (_player.milliHitstun == 0){
        printf("we hit\n");
        _player.health -= amount;
        if (_player.health <= 0){
            printf("Game over I guess?\n");
        }
        _player.milliHitstun = hitstunMilli;
        return 1;
    } else {
        return 0;
    }
}

void movePlayer(){
    _player.e.x += _player.e.changeX;
    _player.e.y += _player.e.changeY;
    _player.e.changeX = 0;
    _player.e.changeY = 0;
}

static void updatePlayerFrame(int delta){
    updateAnimation(_player.e.animation, delta);
    
    //PIZZA - honestly can't remember what all the milliPassed are doing, only the above line may be necessary
    if (_player.e.isMoving){
        _player.e.milliPassed += delta;
        _player.e.milliPassed %= (_player.e.milliPerFrame * _player.e.numFrames);
        
        switch (_player.e.orientation){
            case UP:
                setAnimationLoop(_player.e.animation, 4, 0);
                break;
            case DOWN:
                setAnimationLoop(_player.e.animation, 5, 0);
                break;
            case LEFT:
                setAnimationLoop(_player.e.animation, 6, 0);
                break;
            case RIGHT:
                setAnimationLoop(_player.e.animation, 7, 0);
                break;
            default:
                break;
        }
    } else {
        _player.e.milliPassed = 0;
        
        switch (_player.e.orientation){
            case UP:
                setAnimationLoop(_player.e.animation, 0, 0);
                break;
            case DOWN:
                setAnimationLoop(_player.e.animation, 1, 0);
                break;
            case LEFT:
                setAnimationLoop(_player.e.animation, 2, 0);
                break;
            case RIGHT:
                setAnimationLoop(_player.e.animation, 3, 0);
                break;
            default:
                break;
        }
    }
}

static void updatePlayerOrientation(){
    if (_input.up){
        _player.e.orientation = UP;
    } else if (_input.down) {
        _player.e.orientation = DOWN;
    } else if (_input.left) {
        _player.e.orientation = LEFT;
    } else if (_input.right) {
        _player.e.orientation = RIGHT;
    }
}

static void updatePlayerPosition(int delta){
    _player.e.changeX = 0;
    _player.e.changeY = 0;
    if (_input.up && _input.left){
        _player.e.changeX = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;
        _player.e.changeY = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;

    } else if (_input.up && _input.right){
        _player.e.changeX = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        _player.e.changeY = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;

    } else if (_input.down && _input.left){
        _player.e.changeX = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;
        _player.e.changeY = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);

    } else if (_input.down && _input.right){
        _player.e.changeX = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        _player.e.changeY = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        
	} else if (_input.up == 1){
		_player.e.changeY = _player.e.pixelsPerMilli * delta/1000.0 * -1;

	} else if (_input.down == 1){
		_player.e.changeY = _player.e.pixelsPerMilli * delta/1000.0;

	} else if (_input.left == 1){
		_player.e.changeX = _player.e.pixelsPerMilli * delta/1000.0 * -1;

	} else if (_input.right == 1){
		_player.e.changeX = _player.e.pixelsPerMilli * delta/1000.0;
	}
	
	applyExternalMoves(&_player.e, delta);
}


/////////////////////////////////////////////////
// Drawing
/////////////////////////////////////////////////
void drawPlayer(){
    //draw the player
    drawAnimation(_player.e.sprite, _player.e.animation, _player.e.x + 0.5, _player.e.y + 0.5);
    
    //draw my weapons
    if (_player.equippedAInd >= 0 && _player_weapons.weapons[_player.equippedAInd]->e.active){
    	_player_weapons.weapons[_player.equippedAInd]->e.draw(&_player_weapons.weapons[_player.equippedAInd]->e, 0, 0);
    }
	if (_player.equippedBInd >= 0 && _player_weapons.weapons[_player.equippedBInd]->e.active){
        _player_weapons.weapons[_player.equippedAInd]->e.draw(&_player_weapons.weapons[_player.equippedBInd]->e, 0, 0);
    }
}


/////////////////////////////////////////////////
// Access
/////////////////////////////////////////////////
int isPlayerInteractable(){
    return _input.x && !_inputRead.x;
}
