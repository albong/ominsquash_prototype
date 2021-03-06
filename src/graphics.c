#include "graphics.h"
#include "constants.h"
#include "logging.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static SDL_Surface *loadSurface(char *name);
static SDL_Texture *loadTexture(char *name);
static SDL_Texture *convertToTexture(SDL_Surface *surface);

/////////////////////////////////////////////////
// Variables
/////////////////////////////////////////////////
//static SDL_Surface *screen;
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Surface *screen;

/////////////////////////////////////////////////
// SDL
/////////////////////////////////////////////////
void initSDL(){
    //Initialise SDL - the audio extension library is initialized elsewhere, but we must flag here
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0){
		LOG_ERR("Could not initialize SDL: %s", SDL_GetError());
		exit(1);
	}
    LOG_INF("SDL setup");
	
	//Open a screen
//	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWPALETTE|SDL_DOUBLEBUF);
	window = SDL_CreateWindow("omnisquash", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL){
		LOG_ERR("Couldn't set screen mode to %d x %d: %s", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}
    LOG_INF("Window created");
	
    //PIZZA hacks, get rid of
    screen = SDL_GetWindowSurface(window);

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL){
        LOG_ERR("Couldn't create a renderer!");
        exit(1);
    }
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    LOG_INF("Renderer setup");
    
	//Set the audio rate to 22050, 16 bit stereo, 2 channels and a 4096 byte buffer
//	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) != 0){
//		printf("Could not open audio: %s\n", Mix_GetError());
//		exit(1);
//	}
	
	//Set the screen title 
//	SDL_WM_SetCaption("omnisquash", NULL);

    //Initialize SDL_image
    int imageInitResult = IMG_Init( IMG_INIT_PNG );
    if (!(imageInitResult & IMG_INIT_PNG)){
        LOG_ERR("Couldn't initialize SDL_image!");
        exit(1);
    }
    LOG_INF("SDL_image setup");
}

void stopSDL(){
    SDL_Quit();
    LOG_INF("SDL stopped");
}


/////////////////////////////////////////////////
// Init
/////////////////////////////////////////////////
Sprite *init_Sprite(Sprite *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return NULL;
    }

    self->image = NULL;
    self->frameWidth = 0;
    self->frameHeight = 0;
    self->numFramesPerRow = 0;

    LOG_INF("Sprite at %p initialized", self);
    return self;
}

Animation *init_Animation(Animation *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return NULL;
    }

    self->currLoop = 0;
    self->numLoops = 0;
    self->milliPassed = 0;
    self->currFrame = 0;
    self->loopTotalDuration = NULL;
    self->loopLength = NULL;
    self->repeatLoop = NULL;
    self->frameNumber = NULL;
    self->frameStartTime = NULL;
    
    LOG_INF("Animation at %p initialized", self);
    return self;
}


/////////////////////////////////////////////////
// Free
/////////////////////////////////////////////////
void free_Image(Image *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return;
    }
    
    SDL_FreeSurface(self->surface); //safe to pass NULL
    if (self->isTexture){
        SDL_DestroyTexture(self->texture); //not safe to pass NULL
    }
    
    free(self);
    LOG_INF("Image at %p freed", self);
}

void free_Sprite(Sprite *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return;
    }
    
    free_Image(self->image);
    self->image = NULL;
    self->frameWidth = 0;
    self->frameHeight = 0;
    self->numFramesPerRow = 0;
    free(self);
    LOG_INF("Sprite at %p freed", self);
}

void free_Animation(Animation *self){
    if (self == NULL){
        LOG_WAR("Received null pointer");
        return;
    }

    size_t i;
    for (i = 0; i < self->numLoops; i++){
        free(self->frameNumber[i]);
        free(self->frameStartTime[i]);
        self->frameNumber[i] = NULL;
        self->frameStartTime[i] = NULL;
    }
    free(self->loopLength);
    free(self->loopTotalDuration);
    free(self->repeatLoop);
    free(self->frameNumber);
    free(self->frameStartTime);
    self->loopLength = NULL;
    self->loopTotalDuration = NULL;
    self->repeatLoop = NULL;
    self->frameNumber = NULL;
    self->frameStartTime = NULL;
    
    self->currLoop = 0;
    self->numLoops = 0;
    self->milliPassed = 0;
    self->currFrame = 0;
    
    free(self);
    LOG_INF("Animation at %p freed", self);
}


/////////////////////////////////////////////////
// Loading
/////////////////////////////////////////////////
SDL_Surface *loadSurface(char *name){
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    
    //Load the image
    loadedImage = IMG_Load( name );
    
    //If the image loaded
    if( loadedImage != NULL ){
        //Create an optimized image
//        optimizedImage = SDL_DisplayFormat( loadedImage );
        optimizedImage = SDL_ConvertSurface(loadedImage, screen->format, 0);
        
        //Free the old image
        SDL_FreeSurface( loadedImage );
        
        //If the image was optimized just fine
        if( optimizedImage != NULL ){
            //Map the color key
            Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0xFF, 0, 0xFF); //magic pink
            
            //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
            SDL_SetColorKey(optimizedImage, SDL_TRUE, colorkey);
        } else {
            LOG_WAR("Failed to convert image at %s", name);
        }
    } else {
        LOG_WAR("Failed to load image at %s", name);
    }

    return optimizedImage;
}

SDL_Texture *loadTexture(char *name){
    SDL_Texture *result = NULL;
    
    //load and convert the surface
    SDL_Surface *surface = loadSurface(name);
    if (surface != NULL){
        result = convertToTexture(surface);
        SDL_FreeSurface(surface);
    } else {
        LOG_WAR("SDL surface could not be loaded from %s", name);
    }
    
    LOG_INF("Texture loaded from %s into %p", name, result);
    return result;
}

SDL_Texture *convertToTexture(SDL_Surface *surface){
    return SDL_CreateTextureFromSurface(renderer, surface);
}

Image *getEmptyImage(int width, int height){
    Image *result = malloc(sizeof(Image));
    
    result->width = width;
    result->height = height;
    result->isTexture = 1;
    result->surface = NULL;
    
    //create a blank texture that can be edited
    result->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);;
    
    //if the texture was not created, return null
    if (result->texture == NULL){
        free(result);
        result = NULL;
        LOG_WAR("Failed to create empty texture");
    }
    
    LOG_INF("Empty image created at %p", result);
    return result;
}

Image *loadImage(char *name){
    Image *result = NULL;
    
    //load the texture, malloc and assign to the struct
    SDL_Texture *texture = loadTexture(name);
    if (texture != NULL){
        result = malloc(sizeof(Image));
        result->texture = loadTexture(name);
        result->surface = NULL;
        result->isTexture = 1;
        SDL_QueryTexture(result->texture, NULL, NULL, &(result->width), &(result->height));
    } else {
        LOG_WAR("File not found: %s", name);
    }
    
    LOG_INF("Image loaded from %s into %p", name, result);
    return result;
}

Animation *shallowCopyAnimation(Animation *original){
    if (original == NULL){
        LOG_WAR("Received null pointer");
        return NULL;
    }
    
    //copy the memory
    Animation *result = malloc(sizeof(Animation));
    copyAnimation(original, result);
    
    LOG_INF("Animation at %p shallow copied to %p", original, result);
    return result;
}

Animation *copyAnimation(Animation *source, Animation *dest){
    if (source == NULL || dest == NULL){
        LOG_WAR("Received null pointer");
        return NULL;
    }
    
    memcpy(dest, source, sizeof(Animation));
    
    //we can keep pointers to the loops and data, we just need to have different counters
    dest->currLoop = 0;
    dest->numLoops = 0;
    dest->milliPassed = 0;
    
    LOG_INF("Animation copied from %s to %s", source, dest);
    return dest;
}


/////////////////////////////////////////////////
// Animation Management
/////////////////////////////////////////////////
int updateAnimation(Animation *self, int delta){
    //return 1 on invalid input, 0 on success, 2 if the loop has ended
    if (self == NULL || delta < 0){
        return 1;
    }
    
    //increase our counter of time passed
    self->milliPassed += delta;
    
    //if we run over the total length of this loop, and the loop repeats, mod to restart the count
    int currLoop = self->currLoop;
    if (self->repeatLoop[currLoop] && self->milliPassed >= self->loopTotalDuration[currLoop]){
        self->milliPassed %= self->loopTotalDuration[currLoop];
    }
    
    //go through the frames for this loop until we get to the last one, or find the one that starts after our current time
    size_t i;
    for (i = 0; i < self->loopLength[currLoop]; i++){
        if (self->frameStartTime[currLoop][i] > self->milliPassed){
            break;
        }
    }
    self->currFrame = self->frameNumber[currLoop][i-1];
    
    //if the loop doesn't repeat, then reset milliPassed
    if (!self->repeatLoop[currLoop] && self->milliPassed >= self->loopTotalDuration[currLoop]) {
        self->milliPassed = self->loopTotalDuration[currLoop];
        return 2;
    } else {
        return 0;
    }
}

void setAnimationLoop(Animation *self, int loop, int forceRestart){
    if (self == NULL || loop < 0){
        return;
    }
    
    //if a different loop has been provided, or you're forcing the loop to restart, set everything to 0
    if (self->currLoop != loop || forceRestart){
        self->currLoop = loop;
        self->currFrame = self->frameNumber[self->currLoop][0];
        self->milliPassed = 0;
    }
}


/////////////////////////////////////////////////
// Drawing
/////////////////////////////////////////////////
void drawImage(Image *image, int x, int y){
    //if image is null, do nothing
    if (image == NULL){
        return;
    }

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = image->width;
    dest.h = image->height;

    //do something different depending on if the image is a surface/texture - almost no reason to believe it will ever be a surface
    if (image->isTexture){
        SDL_RenderCopy(renderer, image->texture, NULL, &dest);
    } else {
        SDL_BlitSurface(image->surface, NULL, screen, &dest);
    }
}

void drawImageToImage(Image *src, Image *dst, ImageRect *srcRect, ImageRect *dstRect){
    //safety first
    if (src == NULL || dst == NULL){
        return;
    }
    
    //create SDL_Rects from ImageRects - PIZZA make internal method for this conversion
    SDL_Rect sR, dR;
    SDL_Rect *sRPtr, *dRPtr;
    if (srcRect != NULL){
        sR.x = srcRect->x;
        sR.y = srcRect->y;
        sR.w = srcRect->w;
        sR.h = srcRect->h;
    }
    if (dstRect != NULL){
        dR.x = dstRect->x;
        dR.y = dstRect->y;
        dR.w = dstRect->w;
        dR.h = dstRect->h;
    }
    sRPtr = (srcRect != NULL) ? &sR : NULL;
    dRPtr = (dstRect != NULL) ? &dR : NULL;
    
    //PIZZA - need to handle all these cases?  Surface is never used outside of loading
    if (src->isTexture && dst->isTexture){
        SDL_SetRenderTarget(renderer, dst->texture);
        
        //PIZZA, need to verify can render to texture, check access flags
        SDL_RenderCopy(renderer, src->texture, sRPtr, dRPtr);
        
        SDL_SetRenderTarget(renderer, NULL);
    } else if (src->isTexture && !dst->isTexture){
        printf("NAH\n");
    } else if (src->isTexture && !dst->isTexture){
        printf("NAH\n");
    } else { //both surfaces
        SDL_BlitSurface(src->surface, &sR, dst->surface, &dR);
    }
}

void drawImageSrcDst(Image *image, ImageRect *srcRect, ImageRect *dstRect){
    if (image == NULL){
        return;
    }
    
    //create SDL_Rects from ImageRects - PIZZA make internal method for this conversion
    SDL_Rect sR, dR;
    SDL_Rect *sRPtr, *dRPtr;
    if (srcRect != NULL){
        sR.x = srcRect->x;
        sR.y = srcRect->y;
        sR.w = srcRect->w;
        sR.h = srcRect->h;
    }
    if (dstRect != NULL){
        dR.x = dstRect->x;
        dR.y = dstRect->y;
        dR.w = dstRect->w;
        dR.h = dstRect->h;
    }
    sRPtr = (srcRect != NULL) ? &sR : NULL;
    dRPtr = (dstRect != NULL) ? &dR : NULL;
    
    //draw depending on how the image is stored
    if (image->isTexture){
        SDL_RenderCopy(renderer, image->texture, sRPtr, dRPtr);
    } else {
        SDL_BlitSurface(image->surface, sRPtr, screen, dRPtr);
    }
}

void drawUnfilledRect_S(int x, int y, int w, int h, int r, int g, int b){
    SDL_Rect temp;
    Uint32 color = SDL_MapRGB(screen->format, r, g, b);
    temp = (SDL_Rect){ x, y, 1, h };
    SDL_FillRect(screen, &temp, color);
    temp = (SDL_Rect){ x+w-1, y, 1, h };
    SDL_FillRect(screen, &temp, color);
    temp = (SDL_Rect){ x, y, w, 1 };
    SDL_FillRect(screen, &temp, color);
    temp = (SDL_Rect){ x, y+h-1, w, 1 };
    SDL_FillRect(screen, &temp, color);
}

void drawUnfilledRect_T(int x, int y, int w, int h, int r, int g, int b){
    SDL_Rect temp;
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    temp = (SDL_Rect){ x, y, w, h };
    SDL_RenderDrawRect(renderer, &temp);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

void drawFilledRect_T(int x, int y, int w, int h, int r, int g, int b){
    SDL_Rect temp;
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    temp = (SDL_Rect){ x, y, w, h };
    SDL_RenderFillRect(renderer, &temp);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

void drawAnimation(Sprite *s, Animation *anim, int x, int y){
    //if no animation data given, just draw the image contained in the sprite
    if (anim == NULL){
        drawImage(s->image, x, y);
        return;
    } else if (anim->currFrame == -1){
        return;
    }
    
    //set the src/dst rectangles
    ImageRect src, dest;
    src.x = (anim->currFrame % s->numFramesPerRow) * s->frameWidth;
    src.y = (anim->currFrame / s->numFramesPerRow) * s->frameHeight;
    src.w = s->frameWidth;
    src.h = s->frameHeight;
    
    dest.x = x;
    dest.y = y;
    dest.w = s->frameWidth;
    dest.h = s->frameHeight;
    
    //draw
    drawImageSrcDst(s->image, &src, &dest);
}


/////////////////////////////////////////////////
// Screen Management
/////////////////////////////////////////////////
void clearScreen(){
    SDL_RenderClear(renderer);
}

void bufferToScreen(){
    SDL_RenderPresent(renderer);
}
