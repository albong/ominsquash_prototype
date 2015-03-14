#include "animation.h"

extern SDL_Surface *loadImage(char *);
extern void drawImage(SDL_Surface *, int, int);

void loadAnimation(int id, char *name)
{
	/* Load up the data file that describes the animation */
	
	int i;
	FILE *fp = fopen(name, "rb");
	char frameName[20];
	
	if (fp == NULL)
	{
		printf("Failed to load animation file %s\n", name);
		
		exit(1);
	}
	
	/* Read the frame count */
	
	fscanf(fp, "%d", &animation[id].frameCount);
	
	/* Allocate space for the animation */
	
	animation[id].frame = (SDL_Surface **)malloc(animation[id].frameCount * sizeof(SDL_Surface *));
	
	if (animation[id].frame == NULL)
	{
		printf("Ran out of memory when creating the animation for %s\n", name);
		
		exit(1);
	}
	
	/* Now load up each frame */
	
	for (i=0;i<animation[id].frameCount;i++)
	{
		fscanf(fp, "%s", frameName);
		
		animation[id].frame[i] = loadImage(frameName);
		
		if (animation[id].frame[i] == NULL)
		{
			printf("Failed to load animation frame %s\n", frameName);
			
			exit(1);
		}
	}
}

void freeAnimation(Animation *anim)
{
	int i;
	
	/* Loop through each of the frames and free it */

	if (anim->frame != NULL)
	{
		for (i=0;i<anim->frameCount;i++)
		{
			if (anim->frame[i] != NULL)
			{
				SDL_FreeSurface(anim->frame[i]);
			}
		}
		
		free(anim->frame);
	}
}

void loadAllAnimations()
{
	loadAnimation(ROCK_ANIMATION, "data/anim/rock.dat");
}

void freeAnimations()
{
	int i;
	
	for (i=0;i<MAX_ANIMATIONS;i++)
	{
		freeAnimation(&animation[i]);
	}
}

void drawAnimation(Animation *anim, int index, int x, int y)
{
	drawImage(anim->frame[index], x, y);
}
