#include "Attack.h"
#include "../Clock/Clock.h"
#include "../Global.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../Console/Console.h"

//Game Input
//Game Time
//int BPMProjSpawnTime;

//Only stores 100 values
Projectile pArray[NUMBER_OF_PROJECTILE];
// Keep tracks of how many projectiles are currently in use
// then we update all in-use projectiles
int pCount;
Projectile lArray[LENGTH_OF_LASER];
int lCount;
int laserSpeed = 10;


/*Projectile* Enemy_GetProjectile()
{
	return pArray;
}*/

/* Internal functions */
void _UpdateProjectile();
void _UpdateLaser();
void _ClearLaser();
void _CheckCollision();

void _chooseAttack() //to be in enemy.c
{
	int	randType = Random_Range(1, 15);

	//66.7% chance
	if (randType <= 10)
	{
		//Projectile
	}
	//20% chance
	else if (randType > 10 && randType <= 13)
	{
		//ClusterBomb
	}
	//13.3% chance
	else if (randType > 13 && randType <= 14)
	{
		//Laser
	}
}

void Attack_Init()
{
	pCount = 0;

	// Make all projectiles available for use
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
	{
		pArray[i].debugChar = 'P';
		pArray[i].available = true;
	}

	// Make all projectiles available for use
	lArray[0].available = true;
	lCount = 0;
	for (int i = 0; i < LENGTH_OF_LASER; i++)
	{
		lArray[i].debugChar = 'L';
	}
}

void Attack_FixedUpdate() // put in game.c
{
	_UpdateLaser();

	// If no projectile, dont do anything
	if (pCount == 0)
	{
		return;
	}
	_UpdateProjectile();
	_CheckCollision();
	//Attack_updateLaser();
	//Attack_updateClusterBomb();
}

void Attack_Render() // put in game.c
{
	// If no projectile, dont do anything
	/*if (pCount == 0)
	{
		return;
	}*/

	for (int i = 0; i < pCount; i++)
	{
		if (!pArray[i - 1].visible) return;
		//Print out projectile
		Console_SetRenderBuffer_Char(pArray[i - 1].position.x, pArray[i - 1].position.y, pArray[i - 1].debugChar);
	}

	for (int i = 0; i < LENGTH_OF_LASER; i++)
	{
		if (!lArray[i].visible) return;
		//Print out projectile
		Console_SetRenderBuffer_Char(lArray[i].position.x, lArray[i].position.y, lArray[i].debugChar);
	}
}

void Attack_SpawnProjectile(Vector2d spawnPosition, DIRECTION direction, int speed, int distance)
{
	pCount++;
	// Number of projectiles in game currently
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
	{
		if (pArray[i - 1].available)
		{
			pArray[i - 1].available = false;


			pArray[i - 1].position.x = spawnPosition.x;
			pArray[i - 1].position.y = spawnPosition.y;
			pArray[i - 1].visible = true;
			pArray[i - 1].direction = direction;
			pArray[i - 1].speed = speed;
			pArray[i - 1].distanceToTravel = distance;
			break;
		}
	}
}

void _UpdateProjectile()
{
	for (int i = 0; i < pCount; i++)
	{
		if (pArray[i - 1].distanceToTravel > 0)
		{
			pArray[i - 1].distanceToTravel -= pArray[i - 1].speed;

			switch (pArray[i - 1].direction)
			{
			case UP:
				pArray[i - 1].position.y -= pArray[i - 1].speed;
				break;
			case DOWN:
				pArray[i - 1].position.y += pArray[i - 1].speed;
				break;
			case RIGHT:
				pArray[i - 1].position.x += pArray[i - 1].speed;
				break;
			case LEFT:
				pArray[i - 1].position.x -= pArray[i - 1].speed;
				break;
			default:
				pArray[i - 1].position.y += pArray[i - 1].speed;
			}
		}
	}
}

void _CheckCollision()
{
	//Collision of Projectile to Boundary
	for (int i = 0; i < pCount; i++)
	{
		// If projectile is in use
		if (pArray[i - 1].available)
		{
			return;
		}

		if (pArray[i - 1].position.x > GAME_WIDTH - MAP_OFFSET || pArray[i - 1].position.y > GAME_HEIGHT - MAP_OFFSET
			|| pArray[i - 1].position.x < MAP_OFFSET || pArray[i - 1].position.y < MAP_OFFSET)
		{
			//Hide away projectiles
			pArray[i - 1].visible = false;
			pArray[i - 1].available = true;
			pCount--;
			break;
		}
	}

	// Collision of laser with boundary
	if (lCount > 0)
	{
		if (lArray[lCount - 1].position.x > GAME_WIDTH - MAP_OFFSET || lArray[lCount - 1].position.y > GAME_HEIGHT - MAP_OFFSET ||
			lArray[lCount - 1].position.x < MAP_OFFSET || lArray[lCount - 1].position.y < MAP_OFFSET ||
			lArray[lCount - 1].distanceToTravel < 0)
		{
			_ClearLaser();
		}
	}


	// Collision with player
	sprite *player = Player_GetSprite();
	for (int j = 0; j < player->charCount; j++)
	{
		// Projectile
		for (int i = 1; i < pCount; i++)
		{
			// If projectile is in use
			if (pArray[i - 1].available)
			{
				return;
			}

			if (pArray[i - 1].position.x == player->position[j].x + player->origin.x &&
				pArray[i - 1].position.y == player->position[j].y + player->origin.y)
			{
				//Hide away projectiles
				pArray[i - 1].visible = false;
				pArray[i - 1].available = true;
				pCount--;
				Player_Damage();
				break;
			}
		}

		// Laser
		for (int i = 0; i < lCount; i++)
		{
			// If laser is in use
			if (lArray[i].available)
				return;

			if (lArray[i].position.x == player->position[j].x + player->origin.x &&
				lArray[i].position.y == player->position[j].y + player->origin.y)
			{
				_ClearLaser();
				Player_Damage();
				break;
			}
		}
	}
}

void Attack_SpawnLaser(Vector2d spawnPosition, DIRECTION direction, int length)
{
	if (!lArray[0].available) return;

	lArray[0].position.x = spawnPosition.x;
	lArray[0].position.y = spawnPosition.y;
	lArray[0].direction = direction;
	lArray[0].available = false;
	lArray[0].visible = true;
	lArray[0].distanceToTravel = length;
	lCount = 1;
}

void _UpdateLaser()
{
	if (lArray[0].available) return;

	for (int i = lCount; i < lCount + laserSpeed; i++)
	{
		if (lArray[i].distanceToTravel < 0) return;
		lArray[i] = lArray[i - 1];
		lArray[i].distanceToTravel = lArray[i - 1].distanceToTravel - 1;

		switch (lArray[i - 1].direction)
		{
		case UP:
			lArray[i].position.y = lArray[i - 1].position.y - 1;
			break;
		case DOWN:
			lArray[i].position.y = lArray[i - 1].position.y + 1;
			break;
		case LEFT:
			lArray[i].position.x = lArray[i - 1].position.x - 1;
			break;
		case RIGHT:
			lArray[i].position.x = lArray[i - 1].position.x + 1;
			break;
		}
	}

	lCount += laserSpeed;
}

void _ClearLaser()
{
	for (int i = 0; i < LENGTH_OF_LASER; i++)
	{
		lArray[i].available = true;
		lArray[i].visible = false;
	}
	lCount = 0;
}
