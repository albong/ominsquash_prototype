#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#define MAX_SPRITES 100

// Structs
typedef struct Sprite{
    SDL_Surface *image;
    SDL_Texture *texture;
    int width;
    int height;
} Sprite;

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

typedef struct NewSprite{
    Image *image;
    int frameWidth;
    int frameHeight;
    int numFramesPerRow;
} NewSprite;

//perhaps we ought have an animation list per room to simplify logic?  then you just increment the milliPassed there?
typedef struct SpriteAnimation{
    int currLoop;
    int numLoops;
    int milliPassed;
    int *loopLength;
    int *loopTotalDuration;
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
NewSprite *init_NewSprite(NewSprite *self);
SpriteAnimation *init_SpriteAnimation(SpriteAnimation *self);

// Loading
SDL_Surface *loadSurface(char *name);
SDL_Texture *loadTexture(char *name);
SDL_Texture *convertToTexture(SDL_Surface *surface);
Sprite *loadSprite(char *name);
Sprite *loadAnimatedSprite(char *name, int frameWidth);
SDL_Surface* getEmptySurface(int width, int height);
Image *getEmptyImage(int width, int height);
Image *loadImage(char *name);

// Draw
void drawImage(Image *image, int x, int y);
void drawImageToImage(Image *src, Image *dst, ImageRect *srcRect, ImageRect *dstRect);
void drawImageSrcDst(Image *image, ImageRect *srcRect, ImageRect *dstRect);
void drawImageSrcDst_T(SDL_Texture *image, SDL_Rect src, SDL_Rect dst);
void drawAnimatedSprite(Sprite *s, int frame, int x, int y);
void drawInvertedAnimatedSprite(Sprite *s, int frame, int x, int y, int invert);
void drawUnfilledRect_S(int x, int y, int w, int h, int r, int g, int b);
void drawUnfilledRect_T(int x, int y, int w, int h, int r, int g, int b);
void drawAnimation(NewSprite *s, SpriteAnimation *anim, int x, int y);

// Screen Management
void clearScreen();
void bufferToScreen();

#endif
