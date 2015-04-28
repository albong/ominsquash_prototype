#include "player.h"
#include "graphics.h"
#include "constants.h"
#include "input.h"

static int roomTransition = 0;
static int transitionDirection = -1;
static unsigned totalDelta = 0;

void initPlayer(){
    _player.sprite = getSprite(0);
    _player.orientation = DOWN;    
    
    _player.x = SCREEN_WIDTH / 2;
    _player.y = SCREEN_HEIGHT / 2;
    
    _player.h = _player.sprite->image->h;
    _player.w = 15;
    
    _player.milliPerFrame = 200;
    _player.milliPassed = 0;
    _player.pixelsPerMilli = 70;
    _player.numFrames = 2;
    
    _player.active = 1;
    
    _player.type = PLAYER;
    
    _player.moveHitBox = malloc(sizeof(HitBox) * 1);
    _player.moveHitBox[0].numCircle = 0;
    _player.moveHitBox[0].numRect = 1;
    _player.moveHitBox[0].rects = malloc(sizeof(CollRect) * 1);
    _player.moveHitBox[0].rects[0].x = 4;
    _player.moveHitBox[0].rects[0].y = 5;
    _player.moveHitBox[0].rects[0].w = _player.w-8;
    _player.moveHitBox[0].rects[0].h = _player.sprite->image->h - 5-2;
    
    _player.interactHitBox = malloc(sizeof(HitBox) * 1);
}

void doPlayer(int delta){
    totalDelta += delta;
    if (_player.active == 1){
		if (_input.down || _input.up || _input.left || _input.right){
		    _player.isMoving = 1;
		} else {
		    _player.isMoving = 0;
		}
        
        updatePlayerPosition(delta);
		updatePlayerOrientation();
		updatePlayerFrame(delta);
    } else if (roomTransition){
        double transPercent = totalDelta / MILLI_PER_TRANSITION;
        printf("%f\n", transPercent);
        switch (transitionDirection){
           case ROOM_LEFT:
               _player.x = transPercent * (SCREEN_WIDTH - _player.w);
               break;
            case ROOM_RIGHT:
               _player.x = SCREEN_WIDTH - _player.w - (transPercent * (SCREEN_WIDTH - _player.w));
               break;
            case ROOM_UP:
               _player.y = transPercent * (SCREEN_HEIGHT - _player.h);
               break;
            case ROOM_DOWN:
               _player.y = SCREEN_HEIGHT - _player.h - (transPercent * (SCREEN_HEIGHT - _player.h));
               break;
           default:
                break;
        }
    }
}

static void updatePlayerFrame(int delta){
    if (_player.isMoving){
        _player.milliPassed += delta;
        _player.milliPassed %= (_player.milliPerFrame * _player.numFrames);
    } else {
        _player.milliPassed = 0;
    }
}

static void updatePlayerOrientation(){
    if (_input.up){
        _player.orientation = UP;
    } else if (_input.down) {
        _player.orientation = DOWN;
    } else if (_input.left) {
        _player.orientation = LEFT;
    } else if (_input.right) {
        _player.orientation = RIGHT;
    }
}

static void updatePlayerPosition(int delta){
    _player.changeX = 0;
    _player.changeY = 0;
    if (_input.up && _input.left){
        _player.changeX = _player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;
        _player.changeY = _player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;

    } else if (_input.up && _input.right){
        _player.changeX = _player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        _player.changeY = _player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;

    } else if (_input.down && _input.left){
        _player.changeX = _player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;
        _player.changeY = _player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);

    } else if (_input.down && _input.right){
        _player.changeX = _player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        _player.changeY = _player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        
	} else if (_input.up == 1){
		_player.changeY = _player.pixelsPerMilli * delta/1000.0 * -1;

	} else if (_input.down == 1){
		_player.changeY = _player.pixelsPerMilli * delta/1000.0;

	} else if (_input.left == 1){
		_player.changeX = _player.pixelsPerMilli * delta/1000.0 * -1;

	} else if (_input.right == 1){
		_player.changeX = _player.pixelsPerMilli * delta/1000.0;
	}
}

void movePlayer(){
    _player.x += _player.changeX;
    _player.y += _player.changeY;
    _player.changeX = 0;
    _player.changeY = 0;
}

void setPlayerTransitioning(int direction){
    roomTransition = 1;
    transitionDirection = direction;
    _player.active = 0;
    totalDelta = 0;
}

void stopPlayerTransitioning(){
    roomTransition = 0;
    transitionDirection = -1;
    _player.active = 1;
    totalDelta = 0;
}

void drawPlayer(){
    if (_player.isMoving || roomTransition){
        int frame = ((_player.milliPassed / _player.milliPerFrame) + 1) % _player.numFrames;
        switch (_player.orientation){
            case UP:
                drawAnimatedSprite(_player.sprite, 4 + frame, _player.x + 0.5, _player.y + 0.5);
                break;
            case DOWN:
                drawAnimatedSprite(_player.sprite, 2 + frame, _player.x + 0.5, _player.y + 0.5);
                break;
            case LEFT:
                drawAnimatedSprite(_player.sprite, 0 + frame, _player.x + 0.5, _player.y + 0.5);
                break;
            case RIGHT:
                drawAnimatedSprite(_player.sprite, 6 + frame, _player.x + 0.5, _player.y + 0.5);
                break;
            default:
                break;
        }
    } else {
        switch (_player.orientation){
            case UP:
                drawAnimatedSprite(_player.sprite, 4, _player.x + 0.5, _player.y + 0.5);
                break;
            case DOWN:
                drawAnimatedSprite(_player.sprite, 2, _player.x + 0.5, _player.y + 0.5);
                break;
            case LEFT:
                drawAnimatedSprite(_player.sprite, 0, _player.x + 0.5, _player.y + 0.5);
                break;
            case RIGHT:
                drawAnimatedSprite(_player.sprite, 6, _player.x + 0.5, _player.y + 0.5);
                break;
            default:
                break;
        }
    }
}
