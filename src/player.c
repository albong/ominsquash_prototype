#include "player.h"
#include "graphics.h"
#include "constants.h"
#include "input.h"
#include "weapon.h"
#include "inventory.h"
#include "data_reader.h"
#include "hitbox.h"

#include <stdio.h>

static int roomTransition = 0;
static int transitionDirection = -1;
static unsigned totalDelta = 0;
static const int hitstunMilli = 1000;

static void updatePlayerFrame(int delta);
static void updatePlayerOrientation();
static void updatePlayerPosition(int delta);

/////////////////////////////////////////////////
// Loading / Unloading
/////////////////////////////////////////////////
void initPlayer(){
    init_Entity(&(_player.e));
    _player.e.active = 1;
    
    _player.e.sprite = readSpriteFromFile("data/sprites/00011.sprite", NULL);
    _player.e.animation = readAnimationFromFile("data/animations/00011.animation", NULL);
    _player.e.hitboxes = *(readHitboxesFromFile("data/hitboxes/00011.hitbox", &(_player.e.hitboxes), 0)); //dumb, but visually consistent
    _player.e.orientation = DOWN;    
    
    _player.e.x = SCREEN_WIDTH / 2.0;
    _player.e.y = SCREEN_HEIGHT / 2.0;
    
    _player.e.h = 18;
    _player.e.w = 15;
    
    _player.e.milliPerFrame = 200;
    _player.e.milliPassed = 0;
    _player.e.pixelsPerMilli = 70;
    _player.e.numFrames = 2;
    
    _player.e.active = 1;
    
    _player.e.type = PLAYER;
    
    _player.health = 12;
    _player.shields = 14;
    _player.milliHitstun = 0;
    
    //this will be removed later when this stuff is dynamically set
    //the menu will use the weapons that the player has been given to prevent the player from
    //equipping weapons they don't have
    
    // Weapon *sword = createSword();
    // Weapon *sword = createWeaponById(0);
    // sword->owner = &_player.e;
    // addPlayerWeapon(sword);
    // addWeaponToInventory(sword);
    givePlayerWeapon(0);
    _player.equippedAId = 0;
    
    // Weapon *gun = createWeaponById(1);
    // gun->owner = &_player.e;
    // addPlayerWeapon(gun);
    // addWeaponToInventory(gun);
    givePlayerWeapon(1);
    _player.equippedBId = 1;
}

void termPlayer(){
    term_Entity((Entity *)&_player);
}


/////////////////////////////////////////////////
// Logic
/////////////////////////////////////////////////
void doPlayer(int delta){
    totalDelta += delta;
    if (_player.e.active == 1){
		//if movement key is pressed
        if (checkInput(UP_BUTTON) || checkInput(DOWN_BUTTON) || checkInput(RIGHT_BUTTON) || checkInput(LEFT_BUTTON)){
		    _player.e.isMoving = 1;
		} else {
		    _player.e.isMoving = 0;
		}
		
		//weapon key pressed
		if (_player.equippedAId >= 0){
    		if (checkInput(A_BUTTON) && !_player_weapons[_player.equippedAId].cancelled){
    		    _player_weapons[_player.equippedAId].e.active = 1;
    		    _player_weapons[_player.equippedAId].e.action(_player_weapons + _player.equippedAId, delta);
    		} else if (!checkInput(A_BUTTON)){
    		    _player_weapons[_player.equippedAId].e.active = 0;
    		    _player_weapons[_player.equippedAId].e.milliPassed = 0;
    		    _player_weapons[_player.equippedAId].cancelled = 0;
    		}
        }
		if (_player.equippedBId >= 0){
    		if (checkInput(B_BUTTON) && !_player_weapons[_player.equippedBId].cancelled){
    		    _player_weapons[_player.equippedBId].e.active = 1;
    		    _player_weapons[_player.equippedBId].e.action(_player_weapons + _player.equippedBId, delta);
    		} else if (!checkInput(B_BUTTON)){
    		    _player_weapons[_player.equippedBId].e.active = 0;
    		    _player_weapons[_player.equippedBId].e.milliPassed = 0;
    		    _player_weapons[_player.equippedBId].cancelled = 0;
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

void lockPlayer(){
    _player.e.active = 0;
    totalDelta = 0;
}

void unlockPlayer(){
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
    if (checkInput(UP_BUTTON)){
        _player.e.orientation = UP;
    } else if (checkInput(DOWN_BUTTON)) {
        _player.e.orientation = DOWN;
    } else if (checkInput(LEFT_BUTTON)) {
        _player.e.orientation = LEFT;
    } else if (checkInput(RIGHT_BUTTON)) {
        _player.e.orientation = RIGHT;
    }
}

static void updatePlayerPosition(int delta){
    _player.e.changeX = 0;
    _player.e.changeY = 0;
    if (checkInput(UP_BUTTON) && checkInput(LEFT_BUTTON)){
        _player.e.changeX = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;
        _player.e.changeY = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;

    } else if (checkInput(UP_BUTTON) && checkInput(RIGHT_BUTTON)){
        _player.e.changeX = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        _player.e.changeY = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;

    } else if (checkInput(DOWN_BUTTON) && checkInput(LEFT_BUTTON)){
        _player.e.changeX = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;
        _player.e.changeY = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);

    } else if (checkInput(DOWN_BUTTON) && checkInput(RIGHT_BUTTON)){
        _player.e.changeX = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        _player.e.changeY = _player.e.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        
	} else if (checkInput(UP_BUTTON)){
		_player.e.changeY = _player.e.pixelsPerMilli * delta/1000.0 * -1;

	} else if (checkInput(DOWN_BUTTON)){
		_player.e.changeY = _player.e.pixelsPerMilli * delta/1000.0;

	} else if (checkInput(LEFT_BUTTON)){
		_player.e.changeX = _player.e.pixelsPerMilli * delta/1000.0 * -1;

	} else if (checkInput(RIGHT_BUTTON)){
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
    if (_player.equippedAId >= 0 && _player_weapons[_player.equippedAId].e.active){
    	_player_weapons[_player.equippedAId].e.draw(&_player_weapons[_player.equippedAId].e, 0, 0);
    }
	if (_player.equippedBId >= 0 && _player_weapons[_player.equippedBId].e.active){
        _player_weapons[_player.equippedAId].e.draw(&_player_weapons[_player.equippedBId].e, 0, 0);
    }
}


/////////////////////////////////////////////////
// Access
/////////////////////////////////////////////////
int isPlayerInteractable(){
    return checkInput(X_BUTTON);
}
