#ifndef LOAD_SCREEN_H
#define LOAD_SCREEN_H

void initLoadScreen();
void termLoadScreen();
int doLoadScreen(unsigned delta);
void drawLoadScreen();

void setAreaIdToLoad(int id);

#endif