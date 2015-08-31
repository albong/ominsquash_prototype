#include "enemyloader.h"
#include "enemyconstants.h"
#include "../src/graphics.h"

#include "octorok.h"

static size_t numLoadedSprites = 0;
static size_t *loadedSpriteIds = NULL;
static Sprite **loadedSprites = NULL;

static Sprite *loadEnemySprite(size_t id);
static Sprite *getLoadedSprite(size_t id);

Enemy *createEnemyById(size_t id){
    Enemy *result;
    Sprite *enemySprite = getLoadedSprite(id);
    switch(id){
        case OCTOROK_ID:
            result = createOctorok(enemySprite);
            break;
        default:
            result = NULL;
            break;
    }
    
    return result;
}

Sprite *loadEnemySprite(size_t id){
    Sprite *result;
    
    switch(id){
        case OCTOROK_ID:
            result = loadAnimatedSprite(OCTOROK_SPRITE, OCTOROK_SPRITE_WIDTH);
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

void loadEnemySprites(size_t *ids, size_t count){
    numLoadedSprites = count;
    loadedSpriteIds = malloc(sizeof(size_t) * count);
    loadedSprites = malloc(sizeof(Sprite *) * count);
    
    size_t i;
    for (i = 0; i < count; i++){
        loadedSpriteIds[i] = ids[i];
        loadedSprites[i] = loadEnemySprite(ids[i]);
    }
}

void unloadEnemySprites(){
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

