#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#define MAX_SPRITES 100

//structs
typedef struct Sprite{
    SDL_Surface *image;
    int width;
} Sprite;

//globals
static SDL_Surface *screen;
static TTF_Font *font;

//methods
void initSDL();
void stopSDL();
SDL_Surface *loadImage(char *name);
SDL_Surface* getEmptySurface(int width, int height);
void drawImage(SDL_Surface *image, int x, int y);
void drawImageSrcDst(SDL_Surface *image, SDL_Rect src, SDL_Rect dst);
void drawSprite(Sprite *s, int x, int y);
void drawAnimatedSprite(Sprite *s, int frame, int x, int y);
Sprite *loadAnimatedSprite(char *name, int frameWidth);
void drawUnfilledRect(int x, int y, int w, int h, int r, int g, int b);
void clearScreen();
void bufferToScreen();

#endif
