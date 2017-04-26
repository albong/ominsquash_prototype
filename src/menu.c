#include "menu.h"
#include "graphics.h"
#include "constants.h"
#include "player.h"

static Image *menuImage;

void initMenu(){
    menuImage = loadImage("gfx/menu_placeholder.png");
}

void drawMenu(){
    drawImage(menuImage, (SCREEN_WIDTH / 2) - (menuImage->width / 2), (SCREEN_HEIGHT / 2) - (menuImage->height / 2));
}