#ifndef STRUCTS_H
#define STRUCTS_H

#include "defs.h"

typedef struct Animation
{
	int frameCount;
	SDL_Surface **frame;
} Animation;

typedef struct Game
{
	int score;
	SDL_Surface *screen;
	TTF_Font *font;
} Game;

typedef struct Sprite
{
	SDL_Surface *image;
} Sprite;

typedef struct Sound
{
	Mix_Chunk *effect;
} Sound;

typedef struct Control
{
	int up, down, left, right;
} Control;

typedef struct Star
{
	int x, y, speed;
} Star;

#endif
