#include "graphics.h"
#include "main.h"
#include "entity.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

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
    SDL_PixelFormat *fmt = game.screen->format;
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
    SDL_BlitSurface(image, NULL, game.screen, &dest);
}

void drawImageSrcDst(SDL_Surface *image, SDL_Rect src, SDL_Rect dst){
    SDL_BlitSurface(image, &src, game.screen, &dst);
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
    
    SDL_BlitSurface(s->image, &src, game.screen, &dest);
}

void loadSprite(int index, char *name){
    _sprite_list[index].image = loadImage(name);
    _sprite_list[index].width = _sprite_list[index].image->w;
}

int loadAnimatedSprite(char *name, int frameWidth){
    int i;
    for (i = 0; i < MAX_SPRITES; i++){
        if (!_sprite_list[i].image){
            break;
        }
    }
    _sprite_list[i].image = loadImage(name);
    _sprite_list[i].width = frameWidth;
    return i;
}

Sprite *getSprite(int index){
    return &_sprite_list[index];
}

void draw(){
    //blank out the screen
    SDL_FillRect(game.screen, NULL, 0);
    
    drawCurrentRoom();
    
    drawPlayer();
    
    drawEntities();
    
    //update the buffer
    SDL_Flip(game.screen);
}
