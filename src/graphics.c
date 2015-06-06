#include "graphics.h"
#include "constants.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

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
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWPALETTE|SDL_DOUBLEBUF);
	if (screen == NULL){
		printf("Couldn't set screen mode to %d x %d: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}
	
	//Set the audio rate to 22050, 16 bit stereo, 2 channels and a 4096 byte buffer
//	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) != 0){
//		printf("Could not open audio: %s\n", Mix_GetError());
//		exit(1);
//	}
	
	//Set the screen title 
	SDL_WM_SetCaption("omnisquash", NULL);
}

void stopSDL(){
    SDL_Quit();
}

SDL_Surface *loadImage(char *name){
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    
    //Load the image
    loadedImage = IMG_Load( name );
    
    //If the image loaded
    if( loadedImage != NULL ){
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );
        
        //Free the old image
        SDL_FreeSurface( loadedImage );
        
        //If the image was optimized just fine
        if( optimizedImage != NULL ){
            //Map the color key
            Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0xFF, 0, 0xFF); //magic pink
            
            //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
            SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);
        }
    }

    return optimizedImage;
}

SDL_Surface *getEmptySurface(int width, int height){
    SDL_PixelFormat *fmt = screen->format;
    return SDL_CreateRGBSurface(SDL_HWSURFACE,
        width,
        height,
        fmt->BitsPerPixel,
        fmt->Rmask,
        fmt->Gmask,
        fmt->Bmask,
        fmt->Amask);
}

void drawImage(SDL_Surface *image, int x, int y){
    SDL_Rect dest;
    
    //set the blitting rectangle to the size of the src image
    dest.x = x;
    dest.y = y;
    dest.w = image->w;
    dest.h = image->h;
    
    //blit the entire image onto the screen
    SDL_BlitSurface(image, NULL, screen, &dest);
}

void drawImageSrcDst(SDL_Surface *image, SDL_Rect src, SDL_Rect dst){
    SDL_BlitSurface(image, &src, screen, &dst);
}

void drawSprite(Sprite *s, int x, int y){
    drawImage(s->image, x, y);
}

void drawAnimatedSprite(Sprite *s, int frame, int x, int y){
    SDL_Rect src, dest;
    
    src.x = frame * s->width;
    src.y = 0;
    src.w = s->width;
    src.h = s->image->h;
    
    dest.x = x;
    dest.y = y;
    dest.w = s->width;
    dest.h = s->image->h;
    
    SDL_BlitSurface(s->image, &src, screen, &dest);
}

Sprite *loadAnimatedSprite(char *name, int frameWidth){
    int i;
    Sprite *result = (Sprite *) malloc(sizeof(Sprite));
    result->image = loadImage(name);
    result->width = frameWidth;
    return result;
}

void drawUnfilledRect(int x, int y, int w, int h, int r, int g, int b){
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

void clearScreen(){
    SDL_FillRect(screen, NULL, 0);
}

void bufferToScreen(){
    SDL_Flip(screen);
}
