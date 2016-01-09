#include "entityloader.h"
#include "entityconstants.h"
#include "../src/graphics.h"

#include "skull.h"

static size_t numLoadedSprites = 0;
static size_t *loadedSpriteIds = NULL;
static Sprite **loadedSprites = NULL;

static Sprite *loadEntitySprite(size_t id);
static Sprite *getLoadedSprite(size_t id);

Entity *createEntityById(size_t id){
    Entity *result;
    Sprite *entitySprite = getLoadedSprite(id);
    switch(id){
        case SKULL_ID:
            result = createSkull(entitySprite);
            break;
        default:
            result = NULL;
            break;
    }
    
    return result;
}

Sprite *loadEntitySprite(size_t id){
    Sprite *result;
    
    switch(id){
        case SKULL_ID:
            result = loadAnimatedSprite(SKULL_SPRITE, SKULL_SPRITE_WIDTH);
            break;
        default:
            result = NULL;
            break;
    }
    
    return result;
}

Sprite *getLoadedSprite(size_t id){
    Sprite *result;
    size_t i;
    for (i = 0; i < numLoadedSprites; i++){
        if (loadedSpriteIds[i] == id){
            result = loadedSprites[i];
            break;
        }
    }
    return result;
}

void loadEntitySprites(size_t *ids, size_t count){
    numLoadedSprites = count;
    loadedSpriteIds = malloc(sizeof(size_t) * count);
    loadedSprites = malloc(sizeof(Sprite *) * count);
    
    size_t i;
    for (i = 0; i < count; i++){
        loadedSpriteIds[i] = ids[i];
        loadedSprites[i] = loadEntitySprite(ids[i]);
    }
}

void unloadEntitySprites(){
    size_t i;
    for (i = 0; i < numLoadedSprites; i++){
        //need to add code to graphics.h/c to call SDL_FreeSurface on the surface hiding inside the sprite
        //free(loadedSprites[i]);
    }
    
    free(loadedSpriteIds);
    free(loadedSprites);
    loadedSpriteIds = NULL;
    loadedSprites = NULL;
    numLoadedSprites = 0;
}

