#ifndef INPUT_H
#define INPUT_H

// Enums
/*
Most of the buttons do more or less as labelled on the tin;
'a' and 'b' are for weapons, 'x' is for interacting, 'y' is undecided but maybe jump?
*/
typedef enum {
    UP_BUTTON, DOWN_BUTTON, LEFT_BUTTON, RIGHT_BUTTON, A_BUTTON, B_BUTTON, X_BUTTON, Y_BUTTON, START_BUTTON, SELECT_BUTTON, ESCAPE_BUTTON
} Button;

// Access
void initInput();
void getInput();
void consumeAllInput();
int checkInput(Button b); //checks if a button is being pressed and hasn't been consumed
int checkAndConsumeInput(Button b); //checks if a button is being pressed and marks it as consumed so that other things won't see the press until it is released and respressed

#endif
