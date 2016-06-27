#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#define MAX_SPRITES 100

// Structs
typedef struct Sprite{
    SDL_Surface *image;
    int width;
    int height;
} Sprite;

// Globals
// static SDL_Surface *screen;
static TTF_Font *font;

// SDL 
void initSDL();
void stopSDL();

// Loading
SDL_Surface *loadImage(char *name);
Sprite *loadSprite(char *name);
Sprite *loadAnimatedSprite(char *name, int frameWidth);
SDL_Surface* getEmptySurface(int width, int height);

// Draw
void drawImage(SDL_Surface *image, int x, int y);
void drawImageSrcDst(SDL_Surface *image, SDL_Rect src, SDL_Rect dst);
void drawSprite(Sprite *s, int x, int y);
void drawSpriteSrcDst(Sprite *s, int srcX, int srcY, int w, int h, int dstX, int dstY);
void drawAnimatedSprite(Sprite *s, int frame, int x, int y);
void drawInvertedAnimatedSprite(Sprite *s, int frame, int x, int y, int invert);
void drawUnfilledRect(int x, int y, int w, int h, int r, int g, int b);

// Screen Management
void clearScreen();
void bufferToScreen();

#endif
