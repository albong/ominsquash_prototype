#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define MAX_STARS 200

#define PLAYER_SPEED 3
#define ROCK_SPEED 4

enum
{
	PLAYER_SPRITE,
	MAX_SPRITES
};

enum
{
	ROCK_ANIMATION,
 	MAX_ANIMATIONS
};

enum
{
	EXPLOSION_SOUND,
 	MAX_SOUNDS
};

typedef enum Orientation {
    UP_O,
    DOWN_O,
    LEFT_O,
    RIGHT_O
} Orientation;

#endif
