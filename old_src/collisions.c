#include "collisions.h"

extern void playSound(int);
extern void addRandomParticle(float, float);
extern void addScore(void);

int collision(int, int, int, int, int, int, int, int);

void doCollisions()
{
	int i;

	/* Check each entity against the player, skipping over inactive ones */
	
	if (player.active == 0)
	{
		return;
	}

	for (i=0;i<MAX_ENTITIES;i++)
	{
		if (entity[i].active == 0)
		{
			continue;
		}

		if (collision(entity[i].x, entity[i].y, entity[i].w, entity[i].h, player.x, player.y, player.w, player.h) == 1)
		{
			/* If a collision occured, kill the player */
			
			player.active = 0;
			
			playSound(EXPLOSION_SOUND);
		}
	}
}

/* Very standard 2D collision detection routine */

int collision(int x0, int y0, int w0, int h0, int x2, int y2, int w1, int h1)
{
	int x1 = x0 + w0;
	int y1 = y0 + h0;

	int x3 = x2 + w1;
	int y3 = y2 + h1;

	return !(x1<x2 || x3<x0 || y1<y2 || y3<y0);
}
