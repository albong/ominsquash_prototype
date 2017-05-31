#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#define MAX_SPRITES 100

// Structs
typedef struct Image{
    SDL_Surface *surface;
    SDL_Texture *texture;
    int width;
    int height;
    int isTexture;
} Image;

typedef struct ImageRect{
    int x;
    int y;
    int w;
    int h;
} ImageRect;

typedef struct Sprite{
    Image *image;
    int frameWidth;
    int frameHeight;
    int numFramesPerRow;
} Sprite;

//perhaps we ought have an animation list per room to simplify logic?  then you just increment the milliPassed there?
typedef struct SpriteAnimation{
    int currLoop;
    int numLoops;
    int milliPassed;
    int *loopLength;
    int *loopTotalDuration;
    int *repeatLoop;
    int **frameNumber;
    int **frameStartTime;
} SpriteAnimation;

// Globals
// static SDL_Surface *screen;
static TTF_Font *font;

// SDL 
void initSDL();
void stopSDL();

// Init
Sprite *init_Sprite(Sprite *self);
SpriteAnimation *init_SpriteAnimation(SpriteAnimation *self);

// Loading
Image *getEmptyImage(int width, int height);
Image *loadImage(char *name);
SpriteAnimation *shallowCopySpriteAnimation(SpriteAnimation *original);

// Draw
void drawImage(Image *image, int x, int y);
void drawImageToImage(Image *src, Image *dst, ImageRect *srcRect, ImageRect *dstRect);
void drawImageSrcDst(Image *image, ImageRect *srcRect, ImageRect *dstRect);
void drawUnfilledRect_S(int x, int y, int w, int h, int r, int g, int b);
void drawUnfilledRect_T(int x, int y, int w, int h, int r, int g, int b);
void drawAnimation(Sprite *s, SpriteAnimation *anim, int x, int y);

// Screen Management
void clearScreen();
void bufferToScreen();

#endif
