#include "menu.h"
#include "graphics.h"
#include "constants.h"
#include "player.h"

static Sprite *menuSprite;

void initMenu(){
    menuSprite = loadSprite("gfx/menu_placeholder.png");
}

void drawMenu(){
    drawSprite(menuSprite, (SCREEN_WIDTH / 2) - (menuSprite->width / 2), (SCREEN_HEIGHT / 2) - (menuSprite->height / 2));
}