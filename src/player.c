#include "player.h"
#include "graphics.h"
#include "constants.h"
#include "input.h"

int roomTransition = 0;
int transitionDirection = -1;

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
}

void doPlayer(int delta){
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
        switch (transitionDirection){
           case ROOM_LEFT:
               player.x += delta * TRANSITION_PERCENT * (SCREEN_WIDTH/2 - 2*player.sprite->width);
               break;
            case ROOM_RIGHT:
               player.x -= delta * TRANSITION_PERCENT * (SCREEN_WIDTH/2 - 2*player.sprite->width);
               break;
            case ROOM_UP:
               player.y += delta * TRANSITION_PERCENT * (SCREEN_HEIGHT/2 - 2*player.sprite->image->h);
               break;
            case ROOM_DOWN:
               player.y -= delta * TRANSITION_PERCENT * (SCREEN_HEIGHT/2 - 2*player.sprite->image->h);
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
    if (_input.up && _input.left){
        player.x -= player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        player.y -= player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);

    } else if (_input.up && _input.right){
        player.x += player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        player.y -= player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);

    } else if (_input.down && _input.left){
        player.x -= player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        player.y += player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);

    } else if (_input.down && _input.right){
        player.x += player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        player.y += player.pixelsPerMilli * (delta/1000.0) * (1/SQRT_2);
        
	} else if (_input.up == 1){
		player.y -= player.pixelsPerMilli * delta/1000.0;

	} else if (_input.down == 1){
		player.y += player.pixelsPerMilli * delta/1000.0;

	} else if (_input.left == 1){
		player.x -= player.pixelsPerMilli * delta/1000.0;

	} else if (_input.right == 1){
		player.x += player.pixelsPerMilli * delta/1000.0;
	}
}

void setPlayerTransitioning(int direction){
    roomTransition = 1;
    transitionDirection = direction;
    player.active = 0;
}

void stopPlayerTransitioning(){
    roomTransition = 0;
    transitionDirection = -1;
    player.active = 1;
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
