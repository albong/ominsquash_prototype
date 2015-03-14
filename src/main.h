#ifndef MAIN_H
#define MAIN_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

typedef struct Game {
	int score;
	SDL_Surface *screen;
	TTF_Font *font;
} Game;

Game game;

#endif
