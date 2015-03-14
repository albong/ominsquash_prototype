/* A very basic game.
** This tutorial is a simple rock dodging game
** it uses animation, starfields, SDL_TTF and
** input control. Pressing the arrow keys will
** move the spaceship around, pressing escape
** or closing the window will exit the program.
*/

#include "main.h"
#include "entity.h"

extern void init(char *);
extern void cleanup(void);
extern void getInput(void);
extern void draw(void);
extern void initPlayer(void);
extern void doPlayer(void);
extern void doEntities(void);
extern void loadAllSprites(void);
extern void loadAllSounds(void);
extern void addRock(int);
extern void doCollisions(void);
extern TTF_Font *loadFont(char *, int);
extern void doStars(void);
extern void resetStars(void);
extern void loadAllAnimations(void);
extern void delay(unsigned int);

int main(int argc, char *argv[])
{
	unsigned int frameLimit = SDL_GetTicks() + 16;
	int go = 1;
	
	/* Start up SDL */
	init("Parallel Realities Tutorial 10");
	
	/* Call the cleanup function when the program exits */
	atexit(cleanup);
	
	/* Load all the sprites */
	loadAllSprites();
	
	/* Load all the sounds */
	loadAllSounds();
	
	/* Load all the animations */
	loadAllAnimations();
	
	/* Load the font */
	game.font = loadFont("font/blackWolf.ttf", 16);
	
	/* Intialise the player */
	initPlayer();
	
	/* Reset the stars */
	resetStars();
	
	/* Loop indefinitely for messages */
	while (go == 1)
	{
		/* Get the input */
		getInput();
		
		/* Update the player's position */
		doPlayer();
		
		/* Add a rock */
		addRock(game.score);
		
		/* Update the entities */
		doEntities();
		
		/* Update the stars */
		doStars();
		
		/* Do the collisions */
		doCollisions();
		
		/* Draw everything */
		draw();
		
		/* Increment the player's score while the player's still alive */
		if (player.active == 1)
		{
			game.score += 10;
		}
		
		/* Sleep briefly to stop sucking up all the CPU time */
		delay(frameLimit);
		frameLimit = SDL_GetTicks() + 16;
	}
	
	/* Exit the program */
	
	exit(0);
}
