#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL/SDL.h"

#define MAX_SPRITES 100

//structs
typedef struct Sprite{
    SDL_Surface *image;
    int width;
} Sprite;

//globals
Sprite _sprite_list[MAX_SPRITES];

//methods
SDL_Surface *loadImage(char *name);
SDL_Surface* getEmptySurface(int width, int height);
void drawImage(SDL_Surface *image, int x, int y);
void drawImageSrcDst(SDL_Surface *image, SDL_Rect src, SDL_Rect dst);
void drawSprite(Sprite *s, int x, int y);
void drawAnimatedSprite(Sprite *s, int frame, int x, int y);
void loadSprite(int index, char *name);
int loadAnimatedSprite(char *name, int frameWidth);
Sprite *getSprite(int index);
void draw();

#endif
