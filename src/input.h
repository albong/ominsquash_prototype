#ifndef INPUT_H
#define INPUT_H

// Structs
/*
Most of the buttons do more or less as labelled on the tin;
'a' and 'b' are for weapons, 'x' is for interacting, 'y' is undecided but maybe jump?
*/
typedef struct Control {
    int up;
    int down;
    int left;
    int right;
    int a;
    int b;
    int x;
    int y; 
    int start; 
    int select;
    int escape;
} Control;

// Globals
Control _input;
Control _inputRead;

// Access
void initInput();
void getInput();
void setInputAllRead();

#endif
