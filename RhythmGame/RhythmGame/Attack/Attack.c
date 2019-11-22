#include "Attack.h"
#include "../Clock/Clock.h"
#include "../Global.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../Console/Console.h"
#include "../Enemy/Enemy.h"

//Game Input
//Game Time
//int BPMProjSpawnTime;

//Only stores 100 values
Projectile pArray[NUMBER_OF_PROJECTILE] = { 0 };
// Keep tracks of how many projectiles are currently in use
// then we update all in-use projectiles
Projectile lArray[LENGTH_OF_LASER] = { 0 };
int lCount = 0;
int laserSpeed = 10;

Enemy *enemy;
Player *player;


/*Projectile* Enemy_GetProjectile()
{
	return pArray;
}*/

/* Internal functions */
void _MoveProjectile();
void _MoveLaser();
void _ClearLaser();
void _CheckProjectileBoundary();
void _CheckLaserBoundary();
void _CheckPlayerCollisionWithEnemy();

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
	// Make all projectiles available for use
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
	{
		pArray[i].projectileSprite = Text_CreateSprite();
		Text_Init(&pArray[i].projectileSprite, "..//RhythmGame//$Resources//projectile.txt");
	}

	// Make all projectiles available for lArray[0].available = true;
	for (int i = 0; i < LENGTH_OF_LASER; i++)
	{
		lArray[i].projectileSprite = Text_CreateSprite();
		Text_Init(&lArray[i].projectileSprite, "..//RhythmGame//$Resources//laser.txt");
	}
}

void Attack_Update()
{
	// Get info about player
	enemy = Enemy_GetEnemy();
	player = Player_GetPlayer();
	_CheckPlayerCollisionWithEnemy();
}

void Attack_FixedUpdate() // put in game.c
{


	// Check collision
	_CheckProjectileBoundary();
	_CheckLaserBoundary();
}

void Attack_Render() // put in game.c
{
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
	{
		//Print out projectile
		if (pArray[i].visible)
			Text_Render(&pArray[i].projectileSprite, 0, 0);
	}

	if (lCount > 0)
	{
		for (int i = 0; i < LENGTH_OF_LASER; i++)
		{
			if (!lArray[i].visible) return;
			//Print out projectile
			Text_Render(&lArray[i].projectileSprite, 0, 0);
		}
	}
}

void Attack_SpawnProjectile(Vector2d spawnPosition, DIRECTION direction, int speed, int distance)
{
	_MoveProjectile();
	// Number of projectiles in game currently
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
	{
		if (!pArray[i].visible)
		{
			pArray[i].position.x = spawnPosition.x;
			pArray[i].position.y = spawnPosition.y;
			pArray[i].visible = true;
			pArray[i].direction = direction;
			pArray[i].speed = speed;
			pArray[i].distanceToTravel = distance;
			break;
		}
	}
}

void Attack_SpawnLaser(Vector2d spawnPosition, DIRECTION direction, int length)
{
	_MoveLaser();
	if (lArray[0].visible) return;

	lArray[0].position.x = spawnPosition.x;
	lArray[0].position.y = spawnPosition.y;
	lArray[0].direction = direction;
	lArray[0].visible = true;
	lArray[0].distanceToTravel = length;
	lCount = 1;
}

void _MoveProjectile()
{
	if (!enemy) return;

	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
		if (!pArray[i].visible)
			pArray[i].position = enemy->enemySprite.origin;

	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
	{
		if (pArray[i].distanceToTravel > 0)
		{
			pArray[i].distanceToTravel -= pArray[i].speed;

			switch (pArray[i].direction)
			{
			case UP:
				pArray[i].position.y -= pArray[i].speed;
				break;
			case DOWN:
				pArray[i].position.y += pArray[i].speed;
				break;
			case RIGHT:
				pArray[i].position.x += pArray[i].speed;
				break;
			case LEFT:
				pArray[i].position.x -= pArray[i].speed;
				break;
			default:
				pArray[i].position.y += pArray[i].speed;
			}
		}
		Text_Move(&pArray[i].projectileSprite, pArray[i].position.x, pArray[i].position.y);
	}
}

void _MoveLaser()
{
	for (int i = 0; i < LENGTH_OF_LASER; i++)
		if (!lArray[i].visible)
			lArray[i].position = *enemy->enemySprite.position;

	if (lCount == 0)
		return;

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
		Text_Move(&lArray[i].projectileSprite, lArray[i].position.x, lArray[i].position.y);
	}

	lCount += laserSpeed;
}

void _ClearLaser()
{
	for (int i = 0; i < LENGTH_OF_LASER; i++)
		lArray[i].visible = false;

	lCount = 0;
}

void _CheckProjectileBoundary()
{
	//Collision of Projectile to Boundary
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
	{
		if (pArray[i].position.x > GAME_WIDTH - MAP_OFFSET || pArray[i].position.y > GAME_HEIGHT - MAP_OFFSET ||
			pArray[i].position.x < MAP_OFFSET || pArray[i].position.y < MAP_OFFSET)
		{
			//Hide away projectiles
			pArray[i].visible = false;
			return;
		}
	}
}

void _CheckLaserBoundary()
{
	if (lCount < 1)
		return;

	if (lArray[lCount - 1].position.x > GAME_WIDTH - MAP_OFFSET || lArray[lCount - 1].position.y > GAME_HEIGHT - MAP_OFFSET ||
		lArray[lCount - 1].position.x < MAP_OFFSET || lArray[lCount - 1].position.y < MAP_OFFSET ||
		lArray[lCount - 1].distanceToTravel < 0)
	{
		_ClearLaser();
	}
}

void _CheckPlayerCollisionWithEnemy()
{
	// Player collision 
	if (Player_GetState() == Dash)
	{
		for (int j = 0; j < player->playerSprite.charCount; j++)
		{
			for (int i = 0; i < enemy->enemySprite.charCount; i++)
			{
				if (enemy->enemySprite.origin.x + enemy->enemySprite.position[i].x == player->playerSprite.position[j].x + player->playerSprite.origin.x &&
					enemy->enemySprite.origin.y + enemy->enemySprite.position[i].y == player->playerSprite.position[j].y + player->playerSprite.origin.y)
				{
					Player_ExtendDash();
					// Add score
					break;
				}
			}
		}
	}
	else if (Player_GetState() == Normal)
	{
		// Enemy itself
		for (int j = 0; j < player->playerSprite.charCount; j++)
		{
			for (int i = 0; i < enemy->enemySprite.charCount; i++)
			{
				if (enemy->enemySprite.origin.x + enemy->enemySprite.position[i].x == player->playerSprite.position[j].x + player->playerSprite.origin.x &&
					enemy->enemySprite.origin.y + enemy->enemySprite.position[i].y == player->playerSprite.position[j].y + player->playerSprite.origin.y)
				{
					Player_Damage();
					break;
				}
			}
		}

		//Projectile
		for (int j = 0; j < player->playerSprite.charCount; j++)
		{
			// Projectile
			for (int i = 1; i < NUMBER_OF_PROJECTILE; i++)
			{
				if (pArray[i - 1].position.x == player->playerSprite.position[j].x + player->playerSprite.origin.x &&
					pArray[i - 1].position.y == player->playerSprite.position[j].y + player->playerSprite.origin.y)
				{
					//Hide away projectiles
					pArray[i - 1].visible = false;
					Player_Damage();
					break;
				}
			}
		}

		// Laser
		for (int j = 0; j < player->playerSprite.charCount; j++)
		{
			for (int i = 0; i < lCount; i++)
			{
				if (lArray[i].position.x == player->playerSprite.position[j].x + player->playerSprite.origin.x &&
					lArray[i].position.y == player->playerSprite.position[j].y + player->playerSprite.origin.y)
				{
					_ClearLaser();
					Player_Damage();
					break;
				}
			}
		}
	}
}
