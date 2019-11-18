#include "Attack.h"
#include "../Clock/Clock.h"
#include "../Global.h"
#include "../Map/Map.h"
#include "../Player/Player.h"

//Only stores 100 values
Projectile pArray[NUMBER_OF_PROJECTILE];
<<<<<<< Updated upstream
=======

Laser lArray;
Vector2d laserPosition[LENGTH_OF_LASER];
>>>>>>> Stashed changes

// Keep tracks of how many projectiles are currently in use
// then we update all in-use projectiles
int pCount;

/* Internal functions */
void _UpdateProjectile();
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
		pArray[i].available = true;
}

void Attack_FixedUpdate() // put in game.c
{
	// If no projectile, dont do anything
	if (pCount == 0) return;

	_UpdateProjectile();
	_CheckCollision();
	//Attack_updateLaser();
	//Attack_updateClusterBomb();
}

void Attack_Render() // put in game.c
{
	// If no projectile, dont do anything
	if (pCount == 0) return;

	for (int i = 0; i < pCount; i++)
	{
		if (pArray[i - 1].visible)
		{
			//Print out projectile
			Console_SetRenderBuffer_Char(pArray[i - 1].position.x, pArray[i - 1].position.y, 'P');
		}
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
	//Collision to Boundary
	for (int i = 0; i < pCount; i++)
	{
		// If projectile is in use
		if (pArray[i - 1].available) return;

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

<<<<<<< Updated upstream
=======
	//Collision of Laser to Boundary
	for (int i = 0; i < lArray.distanceToTravel; i++)
	{
		if (laserPosition[i].x > GAME_WIDTH - MAP_OFFSET || laserPosition[i].y > GAME_HEIGHT - MAP_OFFSET
			|| laserPosition[i].x < MAP_OFFSET || laserPosition[i].y < MAP_OFFSET)
		{
			//Hide away projectiles
			lArray.visible = false;
			lArray.available = true;
			laserPosition[i].x = 500;
			laserPosition[i].y = 500;
			lArray.distanceToTravel--;
			break;
		}
	}


>>>>>>> Stashed changes
	// Collision with player
	sprite *player = Player_GetSprite();
	for (int j = 0; j < player->charCount; j++)
	{
		for (int i = 0; i < pCount; i++)
		{
			// If projectile is in use
			if (pArray[i - 1].available) return;

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
	}
}

/*void Attack_spawnLaser()
{

}

void Attack_updateLaser()
{
<<<<<<< Updated upstream

=======
	for (int i = 0; i < lArray.distanceToTravel; i++)
	{
		if (lArray.available)
		{
			//if laser has not been used, don't do anything.
			return;
		}
  		switch (lArray.direction)
		{
		case UP:
			laserPosition[i].x = lArray.position.x;
			laserPosition[i].y = lArray.position.y - i;
			break;
		case DOWN:
			laserPosition[i].x = lArray.position.x;
			laserPosition[i].y = lArray.position.y + i;
			break;
		case RIGHT:
			laserPosition[i].y = lArray.position.y;
			laserPosition[i].x = lArray.position.x + i;
			break;
		case LEFT:
			laserPosition[i].y = lArray.position.y;
			laserPosition[i].x = lArray.position.x - i;
			break;
		default:
			laserPosition[i].x = lArray.position.x;
			laserPosition[i].y = lArray.position.y + i;
		}
	}
>>>>>>> Stashed changes
}

void Attack_spawnClusterBomb()
{

}

void Attack_updateClusterBomb()
{

}*/