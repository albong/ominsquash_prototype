#include "graphics.h"
#include "constants.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdint.h>
#include <stdio.h>


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
    //Initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0){
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	
	//Initialise SDL_TTF 
	if (TTF_Init() < 0){
		printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
		exit(1);
	}
	
	//Open a screen
//	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWPALETTE|SDL_DOUBLEBUF);
	window = SDL_CreateWindow("omnisquash", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL){
		printf("Couldn't set screen mode to %d x %d: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}
	
    //PIZZA hacks, get rid of
    screen = SDL_GetWindowSurface(window);

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL){
        printf("Couldn't create a renderer!\n");
        exit(1);
    }
    
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    
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
        printf("Couldn't initialize SDL_image!\n");
        exit(1);
    }
}

void stopSDL(){
    SDL_Quit();
}


/////////////////////////////////////////////////
// Init
/////////////////////////////////////////////////
NewSprite *init_NewSprite(NewSprite *self){
    if (self == NULL){
        return NULL;
    }

    self->image = NULL;
    self->frameWidth = 0;
    self->frameHeight = 0;
    self->numFramesPerRow = 0;

    return self;
}

SpriteAnimation *init_SpriteAnimation(SpriteAnimation *self){
    if (self == NULL){
        return NULL;
    }

    self->numLoops = 0;
    self->milliPassed = 0;
    self->loopLength = NULL;
    self->frameNumber = NULL;
    self->frameDuration = NULL;
    
    return self;
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
        }
    }

    return optimizedImage;
}

SDL_Texture *loadTexture(char *name){
    SDL_Surface *surface = loadSurface(name);
    SDL_Texture *result = convertToTexture(surface);
    SDL_FreeSurface(surface);
    return result;
}

SDL_Texture *convertToTexture(SDL_Surface *surface){
    return SDL_CreateTextureFromSurface(renderer, surface);
}

Sprite *loadSprite(char *name){
    int i;
    Sprite *result = (Sprite *) malloc(sizeof(Sprite));
    result->image = loadSurface(name);
    result->texture = convertToTexture(result->image);
    result->width = result->image->w;
    result->height = result->image->h;
    return result;
}

Sprite *loadAnimatedSprite(char *name, int frameWidth){
    int i;
    Sprite *result = (Sprite *) malloc(sizeof(Sprite));
    result->image = loadSurface(name);
    result->texture = convertToTexture(result->image);
    result->width = frameWidth;
    result->height = result->image->h;
    return result;
}

SDL_Surface *getEmptySurface(int width, int height){
    SDL_PixelFormat *fmt = screen->format;
    return SDL_CreateRGBSurface(0,
        width,
        height,
        fmt->BitsPerPixel,
        fmt->Rmask,
        fmt->Gmask,
        fmt->Bmask,
        fmt->Amask);
}

Image *loadImage(char *name){
    Image *result = malloc(sizeof(Image));
    
    result->texture = loadTexture(name);
    result->surface = NULL;
    result->isTexture = 1;
    SDL_QueryTexture(result->texture, NULL, NULL, &(result->width), &(result->height));
    
    return result;
}


/////////////////////////////////////////////////
// Drawing
/////////////////////////////////////////////////
void drawImage_S(SDL_Surface *image, int x, int y){
    SDL_Rect dest;
    
    //set the blitting rectangle to the size of the src image
    dest.x = x;
    dest.y = y;
    dest.w = image->w;
    dest.h = image->h;
    
    //blit the entire image onto the screen
    SDL_BlitSurface(image, NULL, screen, &dest);
}

void drawImage_T(SDL_Texture *image, int x, int y){
    SDL_Rect dest;
    
    //set the blitting rectangle to the size of the src image
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);
    
    //blit the entire image onto the screen
    SDL_RenderCopy(renderer, image, NULL, &dest);
}

void drawImageSrcDst_S(SDL_Surface *image, SDL_Rect src, SDL_Rect dst){
    SDL_BlitSurface(image, &src, screen, &dst);
}

void drawImageSrcDst_T(SDL_Texture *image, SDL_Rect src, SDL_Rect dst){
    SDL_RenderCopy(renderer, image, &src, &dst);
}

void drawSprite(Sprite *s, int x, int y){
    drawImage_T(s->texture, x, y);    
}

void drawSpriteSrcDst(Sprite *s, int srcX, int srcY, int w, int h, int dstX, int dstY){
    SDL_Rect src, dest;
    
    src.x = srcX;
    src.y = srcY;
    src.w = w;
    src.h = h;
    
    dest.x = dstX;
    dest.y = dstY;
    dest.w = w;
    dest.h = h;
    
    drawImageSrcDst_T(s->texture, src, dest);
}

void drawAnimatedSprite(Sprite *s, int frame, int x, int y){
    SDL_Rect src, dest;
    
    src.x = frame * s->width;
    src.y = 0;
    src.w = s->width;
    SDL_QueryTexture(s->texture, NULL, NULL, NULL, &src.h);
    
    dest.x = x;
    dest.y = y;
    dest.w = s->width;
    SDL_QueryTexture(s->texture, NULL, NULL, NULL, &dest.h);
    
    drawImageSrcDst_T(s->texture, src, dest);
}

/*
Given the processing cost associated with this, it may make more sense to store an inverted version of the sprite in memory?
*/
void drawInvertedAnimatedSprite(Sprite *s, int frame, int x, int y, int invert){
    if (!invert){
        drawAnimatedSprite(s, frame, x, y);
        return;
    }
    
    return;
    SDL_Rect src, dest;
    
    src.x = frame * s->width;
    src.y = 0;
    src.w = s->width;
    src.h = s->image->h;
    
    dest.x = x;
    dest.y = y;
    dest.w = s->width;
    dest.h = s->image->h;
    
    size_t px, py;
    uint32_t *pixels = (uint32_t *)s->image->pixels;
    uint8_t r, g, b;
    for (px = 0; px < s->image->w; px++){
        for (py = 0; py < s->image->h; py++){
            SDL_GetRGB(pixels[(py * s->image->w) + px], screen->format, &r, &g, &b);
            if (r != 255 && g != 0 && b != 255){
                pixels[(py * s->image->w) + px] = SDL_MapRGB(screen->format, 255-r, 255-g, 255-b);
            }
        }
    }
    
    SDL_BlitSurface(s->image, &src, screen, &dest);
    
    //revert the inversion
    for (px = 0; px < s->image->w; px++){
        for (py = 0; py < s->image->h; py++){
            SDL_GetRGB(pixels[(py * s->image->w) + px], screen->format, &r, &g, &b);
            if (r != 255 && g != 0 && b != 255){
                pixels[(py * s->image->w) + px] = SDL_MapRGB(screen->format, 255-r, 255-g, 255-b);
            }
        }
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
//    Uint32 color = SDL_MapRGB(screen->format, r, g, b);
//    temp = (SDL_Rect){ x, y, 1, h };
//    SDL_FillRect(screen, &temp, color);
//    temp = (SDL_Rect){ x+w-1, y, 1, h };
//    SDL_FillRect(screen, &temp, color);
//    temp = (SDL_Rect){ x, y, w, 1 };
//    SDL_FillRect(screen, &temp, color);
//    temp = (SDL_Rect){ x, y+h-1, w, 1 };
//    SDL_FillRect(screen, &temp, color);
    temp = (SDL_Rect){ x, y, w, h };
    SDL_RenderDrawRect(renderer, &temp);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
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
