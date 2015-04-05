#include "player.h"
#include "graphics.h"
#include "constants.h"
#include "input.h"

static int roomTransition = 0;
static int transitionDirection = -1;
static unsigned totalDelta = 0;

void initPlayer(){
    player.sprite = getSprite(0);
    player.orientation = DOWN;    
    
    player.x = SCREEN_WIDTH / 2;
    player.y = SCREEN_HEIGHT / 2;
    
    player.h = player.sprite->image->h;
    player.w = 15;
    
    player.milliPerFrame = 200;
    player.milliPassed = 0;
    player.pixelsPerMilli = 70;
    player.numFrames = 2;
    
    player.active = 1;
    
    player.type = PLAYER;
    
    player.moveHitBox = malloc(sizeof(HitBox) * 1);
    player.moveHitBox[0].numCircle = 0;
    player.moveHitBox[0].numRect = 1;
    player.moveHitBox[0].rects = malloc(sizeof(CollRect) * 1);
    player.moveHitBox[0].rects[0].x = 0;
    player.moveHitBox[0].rects[0].y = 5;
    player.moveHitBox[0].rects[0].w = player.w;
    player.moveHitBox[0].rects[0].y = player.sprite->image->h - 5;
    
    player.interactHitBox = malloc(sizeof(HitBox) * 1);
}

void doPlayer(int delta){
    totalDelta += delta;
    if (player.active == 1){
		if (_input.down || _input.up || _input.left || _input.right){
		    player.isMoving = 1;
		} else {
		    player.isMoving = 0;
		}
        
        updatePlayerPosition(delta);
		updatePlayerOrientation();
		updatePlayerFrame(delta);
    } else if (roomTransition){
        double transPercent = totalDelta / MILLI_PER_TRANSITION;
        switch (transitionDirection){
           case ROOM_LEFT:
               player.x = transPercent * (SCREEN_WIDTH - player.w);
               break;
            case ROOM_RIGHT:
               player.x = SCREEN_WIDTH - player.w - (transPercent * (SCREEN_WIDTH - player.w));
               break;
            case ROOM_UP:
               player.y = transPercent * (SCREEN_HEIGHT - player.h);
               break;
            case ROOM_DOWN:
               player.y = SCREEN_HEIGHT - player.h - (transPercent * (SCREEN_HEIGHT - player.h));
               break;
           default:
                break;
        }
    }
}

static void updatePlayerFrame(int delta){
    if (player.isMoving){
        player.milliPassed += delta;
        player.milliPassed %= (player.milliPerFrame * player.numFrames);
    } else {
        player.milliPassed = 0;
    }
}

static void updatePlayerOrientation(){
    if (_input.up){
        player.orientation = UP;
    } else if (_input.down) {
        player.orientation = DOWN;
    } else if (_input.left) {
        player.orientation = LEFT;
    } else if (_input.right) {
        player.orientation = RIGHT;
    }
}

static void updatePlayerPosition(int delta){
//    if (_input.up && _input.left){
//        player.x -= player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
//        player.y -= player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
//
//    } else if (_input.up && _input.right){
//        player.x += player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
//        player.y -= player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
//
//    } else if (_input.down && _input.left){
//        player.x -= player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
//        player.y += player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
//
//    } else if (_input.down && _input.right){
//        player.x += player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
//        player.y += player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
//        
//	} else if (_input.up == 1){
//		player.y -= player.pixelsPerMilli * delta/1000.0;
//
//	} else if (_input.down == 1){
//		player.y += player.pixelsPerMilli * delta/1000.0;
//
//	} else if (_input.left == 1){
//		player.x -= player.pixelsPerMilli * delta/1000.0;
//
//	} else if (_input.right == 1){
//		player.x += player.pixelsPerMilli * delta/1000.0;
//	}
    if (_input.up && _input.left){
        player.changeX = player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;
        player.changeY = player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;

    } else if (_input.up && _input.right){
        player.changeX = player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        player.changeY = player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;

    } else if (_input.down && _input.left){
        player.changeX = player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2) * -1;
        player.changeY = player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);

    } else if (_input.down && _input.right){
        player.changeX = player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        player.changeY = player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        
	} else if (_input.up == 1){
		player.changeY = player.pixelsPerMilli * delta/1000.0 * -1;

	} else if (_input.down == 1){
		player.changeY = player.pixelsPerMilli * delta/1000.0;

	} else if (_input.left == 1){
		player.changeX = player.pixelsPerMilli * delta/1000.0 * -1;

	} else if (_input.right == 1){
		player.changeX = player.pixelsPerMilli * delta/1000.0;
	}
}

void movePlayer(){
    player.x += player.changeX;
    player.y += player.changeY;
    player.changeX = 0;
    player.changeY = 0;
}

void setPlayerTransitioning(int direction){
    roomTransition = 1;
    transitionDirection = direction;
    player.active = 0;
    totalDelta = 0;
}

void stopPlayerTransitioning(){
    roomTransition = 0;
    transitionDirection = -1;
    player.active = 1;
    totalDelta = 0;
}

void drawPlayer(){
    if (player.isMoving || roomTransition){
        int frame = ((player.milliPassed / player.milliPerFrame) + 1) % player.numFrames;
        switch (player.orientation){
            case UP:
                drawAnimatedSprite(player.sprite, 4 + frame, player.x + 0.5, player.y + 0.5);
                break;
            case DOWN:
                drawAnimatedSprite(player.sprite, 2 + frame, player.x + 0.5, player.y + 0.5);
                break;
            case LEFT:
                drawAnimatedSprite(player.sprite, 0 + frame, player.x + 0.5, player.y + 0.5);
                break;
            case RIGHT:
                drawAnimatedSprite(player.sprite, 6 + frame, player.x + 0.5, player.y + 0.5);
                break;
            default:
                break;
        }
    } else {
        switch (player.orientation){
            case UP:
                drawAnimatedSprite(player.sprite, 4, player.x + 0.5, player.y + 0.5);
                break;
            case DOWN:
                drawAnimatedSprite(player.sprite, 2, player.x + 0.5, player.y + 0.5);
                break;
            case LEFT:
                drawAnimatedSprite(player.sprite, 0, player.x + 0.5, player.y + 0.5);
                break;
            case RIGHT:
                drawAnimatedSprite(player.sprite, 6, player.x + 0.5, player.y + 0.5);
                break;
            default:
                break;
        }
    }
}
