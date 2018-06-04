#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL2/SDL.h"

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
typedef struct Animation{
    int currLoop;
    int numLoops;
    int milliPassed;
    int currFrame;
    int *loopLength;
    int *loopTotalDuration;
    int *repeatLoop;
    int **frameNumber;
    int **frameStartTime;
} Animation;

// Globals
// static SDL_Surface *screen;

// SDL 
void initSDL();
void stopSDL();

// Init
Sprite *init_Sprite(Sprite *self);
Animation *init_Animation(Animation *self);

// Free
void free_Image(Image *self);
void free_Sprite(Sprite *self);
void free_Animation(Animation *self);

// Loading
Image *getEmptyImage(int width, int height);
Image *loadImage(char *name);
Animation *shallowCopyAnimation(Animation *original);
Animation *copyAnimation(Animation *source, Animation *dest);

// Animation Management
int updateAnimation(Animation *self, int delta);
void setAnimationLoop(Animation *self, int loop, int forceRestart);

// Draw
void drawImage(Image *image, int x, int y);
void drawImageToImage(Image *src, Image *dst, ImageRect *srcRect, ImageRect *dstRect);
void drawImageSrcDst(Image *image, ImageRect *srcRect, ImageRect *dstRect);
void drawUnfilledRect_S(int x, int y, int w, int h, int r, int g, int b);
void drawUnfilledRect_T(int x, int y, int w, int h, int r, int g, int b);
void drawFilledRect_T(int x, int y, int w, int h, int r, int g, int b);
void drawAnimation(Sprite *s, Animation *anim, int x, int y);

// Screen Management
void clearScreen();
void bufferToScreen();

#endif
