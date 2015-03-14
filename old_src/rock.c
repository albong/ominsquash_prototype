#include "rock.h"

extern int getFreeEntity(void);
extern void drawAnimatedEntity(void);
extern SDL_Surface *getSprite(int);

static void moveRock(void);

void addRock(int score)
{
	int i, rockChance;
	
	/* The chance of a rock being added depends upon the player's score */
	
	if (score < 10000)
	{
		rockChance = rand() % 60;
	}
	
	else if (score < 20000)
	{
		rockChance = rand() % 45;
	}
	
	else if (score < 30000)
	{
		rockChance = rand() % 30;
	}
	
	else
	{
		rockChance = rand() % 15;
	}
	
	if (rockChance != 0)
	{
		return;
	}
	
	i = getFreeEntity();
	
	if (i == -1)
	{
		return;
	}
	
	entity[i].x = SCREEN_WIDTH + rand() % 100;
	entity[i].y = rand() % SCREEN_HEIGHT;
	entity[i].action = &moveRock;
	entity[i].speed = 1 + (rand() % 5);
	entity[i].draw = &drawAnimatedEntity;
	entity[i].animID = ROCK_ANIMATION;
	entity[i].currentFrame = 0;
	entity[i].frameTimer = 3;
	entity[i].w = animation[entity[i].animID].frame[entity[i].currentFrame]->w;
	entity[i].h = animation[entity[i].animID].frame[entity[i].currentFrame]->h;
}

static void moveRock(void)
{
	/* Move the rock from right to left, once it goes off the screen, kill it */
	
	self->x -= self->speed;
	
	if (self->x <= -self->w)
	{
		self->active = 0;
	}
	
	/* Update the animation frame */
	
	self->frameTimer--;
	
	if (self->frameTimer <= 0)
	{
		self->frameTimer = 3;
		self->currentFrame++;
		
		if (self->currentFrame >= animation[self->animID].frameCount)
		{
			self->currentFrame = 0;
		}
	}
}
