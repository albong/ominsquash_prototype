#include "menu.h"
#include "graphics.h"
#include "constants.h"
#include "player.h"

static Image *menuImage;
static Entity *menuEntity;

void initMenu(){
    menuImage = loadImage("gfx/menu_placeholder.png");
    menuEntity = init_Entity(malloc(sizeof(Entity)));
    
}

void termMenu(){
    free_Image(menuImage);
}

int doMenu(unsigned delta){
    
}

void drawMenu(){
    drawImage(menuImage, (SCREEN_WIDTH / 2) - (menuImage->width / 2), (SCREEN_HEIGHT / 2) - (menuImage->height / 2));
}