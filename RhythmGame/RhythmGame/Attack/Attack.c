#include "Attack.h"
#include "../Clock/Clock.h"
#include "../Global.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../Console/Console.h"
#include "../Enemy/Enemy.h"
#include "../Audio/AudioEngine.h"
#include <stdlib.h>

#define PROJECTILE_SPEED 0.02
#define PROJECTILE_SPEED_FAST 0.05
#define LASER_UPDATE_SPEED 100.0

// Keep tracks of how many projectiles are currently in use
// then we update all in-use projectiles
Projectile *pArray;
Laser *lArray;

/* Internal functions */
void _MoveProjectile();
void _CheckProjectileCollision();
void _MoveLaser();
void _CheckLaserCollision();
void _ClearLaser(int i);

void Attack_Init()
{
	// Malloc memory for attacks
	pArray = (Projectile *)malloc(sizeof(Projectile) * NUMBER_OF_PROJECTILE);
	lArray = (Laser *)malloc(sizeof(Laser) * NUMBER_OF_LASER);

	// Init projectiles sprites
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
	{
		pArray[i].active = false;
		pArray[i].position.x = -1;
		pArray[i].position.y = -1;
		pArray[i].projectileSprite = Text_CreateSprite();
		Text_Init(&pArray[i].projectileSprite, "..//RhythmGame//$Resources//projectile.txt");
	}

	// Init laser sprites
	for (int i = 0; i < NUMBER_OF_LASER; i++)
	{
		// From i = 1 onwards
		lArray[i].active = false;
		lArray[i].spawnSprite = Text_CreateSprite();
		Text_Init(&lArray[i].spawnSprite, "..//RhythmGame//$Resources//laserspawn.txt");
		for (int j = 0; j < LENGTH_OF_LASER; j++)
		{
			lArray[i].laserSprite[j] = Text_CreateSprite();
			Text_Init(&lArray[i].laserSprite[j], "..//RhythmGame//$Resources//laser.txt");
		}
	}
}

void Attack_Update()
{
	// Projectile
	_CheckProjectileCollision();
	_MoveProjectile();

	// Must update laser movement before collision!
	_MoveLaser();
	_CheckLaserCollision();
}

void Attack_Render() // put in game.c
{
	//Print out projectile
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
		if (pArray[i].active)
			Text_Render(&pArray[i].projectileSprite, 0, 0);

	// Print out laser
	for (int i = 0; i < NUMBER_OF_LASER; i++)
	{
		if (lArray[i].active)
		{
			// Laser body
			for (int j = 0; j < lArray[i].laserIndex; j++)
				Text_Render(&lArray[i].laserSprite[j], 0, 0);

			// Spawn should overlap laser body
			Text_Render(&lArray[i].spawnSprite, 0, 0);
		}
	}
}

void Attack_Cleanup()
{
	free(pArray);
	free(lArray);
}

void Attack_Spawn(ATTACKTYPE type, Vector2d spawnPosition, DIRECTION direction)
{
	switch (type)
	{
	case PROJECTILE:
		for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
		{
			// Find a projectile that is not in use
			if (pArray[i].active) continue;
			pArray[i].position = spawnPosition;
			pArray[i].direction = direction;
			pArray[i].active = true;
			return;
		}
	case LASER:
		for (int i = 0; i < NUMBER_OF_LASER; i++)
		{
			// Find a laser that is not in use
			if (lArray[i].active) continue;
			Text_Move(&lArray[i].spawnSprite, spawnPosition.x, spawnPosition.y);
			lArray[i].laserPosition = spawnPosition;
			lArray[i].direction = direction;
			lArray[i].tick = LASER_UPDATE_SPEED;
			lArray[i].laserIndex = 0;
			lArray[i].active = true;
			return;
		}
	}
}

void _MoveProjectile()
{
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
	{
		// If projectile not in use, don't update it
		if (!pArray[i].active) continue;

		// Sync speed to projectile
		double speed = (Audio_GetSpectrum(1) ? PROJECTILE_SPEED_FAST : PROJECTILE_SPEED) * Clock_GetDeltaTime();

		// Move using euler
		switch (pArray[i].direction)
		{
		case UP:
			pArray[i].position.eulerY -= speed;
			break;
		case DOWN:
			pArray[i].position.eulerY += speed;
			break;
		case LEFT:
			pArray[i].position.eulerX -= speed;
			break;
		case RIGHT:
			pArray[i].position.eulerX += speed;
			break;
		case TOPLEFT:
			pArray[i].position.eulerY -= speed / 1.4;
			pArray[i].position.eulerX -= speed / 1.4;
			break;
		case TOPRIGHT:
			pArray[i].position.eulerY -= speed / 1.4;
			pArray[i].position.eulerX += speed / 1.4;
			break;
		case BOTTOMLEFT:
			pArray[i].position.eulerY += speed / 1.4;
			pArray[i].position.eulerX -= speed / 1.4;
			break;
		case BOTTOMRIGHT:
			pArray[i].position.eulerY += speed / 1.4;
			pArray[i].position.eulerX += speed / 1.4;
			break;
		}

		// Move sprite
		pArray[i].position.x = (int)pArray[i].position.eulerX;
		pArray[i].position.y = (int)pArray[i].position.eulerY;
		Text_Move(&pArray[i].projectileSprite, pArray[i].position.x, pArray[i].position.y);
	}
}

void _CheckProjectileCollision()
{
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
	{
		// If projectile not in use, don't update it
		if (!pArray[i].active) continue;

		// Check against border
		if (pArray[i].position.x <= Map_GetOrigin().x ||
			pArray[i].position.x >= Map_GetEnd().x ||
			pArray[i].position.y <= Map_GetOrigin().y ||
			pArray[i].position.y >= Map_GetEnd().y)
		{
			pArray[i].active = false;
			pArray[i].position.x = -3;
			pArray[i].position.y = -3;
		}
	}
}

void _MoveLaser()
{
	for (int i = 0; i < NUMBER_OF_LASER; i++)
	{
		// Update the active laser only
		if (!lArray[i].active) continue;

		lArray[i].tick -= Clock_GetDeltaTime();

		if (lArray[i].tick <= 0.0)
		{
			// Reset tick
			lArray[i].tick = LASER_UPDATE_SPEED;

			switch (lArray[i].direction)
			{
			default:
				lArray[i].laserPosition.y--;
				break;
			case DOWN:
				lArray[i].laserPosition.y++;
				break;
			case LEFT:
				lArray[i].laserPosition.x--;
				break;
			case RIGHT:
				lArray[i].laserPosition.x++;
				break;
			}
			Text_Move(&lArray[i].laserSprite[lArray[i].laserIndex], lArray[i].laserPosition.x, lArray[i].laserPosition.y);
			lArray[i].laserIndex++;
		}
	}
}

void _CheckLaserCollision()
{
	for (int i = 0; i < NUMBER_OF_LASER; i++)
	{
		// If not in use
		if (!lArray[i].active) continue;

		// Check against border
		if (lArray[i].laserPosition.x <= Map_GetOrigin().x ||
			lArray[i].laserPosition.x >= Map_GetEnd().x ||
			lArray[i].laserPosition.y <= Map_GetOrigin().y ||
			lArray[i].laserPosition.y >= Map_GetEnd().y)
		{
			_ClearLaser(i);
		}
	}
}

void _ClearLaser(int i)
{
	lArray[i].active = false;
	lArray[i].laserPosition.x = -3;
	lArray[i].laserPosition.y = -3;
}

//void _MoveLaser()
//{
//	for (int i = 0; i < LENGTH_OF_LASER; i++)
//		if (!lArray[i].active)
//			lArray[i].position = *enemy->enemySprite.position;
//
//	if (lCount == 0)
//		return;
//
//	for (int i = lCount; i < lCount + laserSpeed; i++)
//	{
//		if (lArray[i - 1].distanceToTravel < 0) return;
//		lArray[i] = lArray[i - 1];
//		lArray[i].distanceToTravel = lArray[i - 1].distanceToTravel - 1;
//
//		switch (lArray[i - 1].direction)
//		{
//		case UP:
//			lArray[i].position.y = lArray[i - 1].position.y - 1;
//			break;
//		default:
//			lArray[i].position.y = lArray[i - 1].position.y + 1;
//			break;
//		case LEFT:
//			lArray[i].position.x = lArray[i - 1].position.x - 1;
//			break;
//		case RIGHT:
//			lArray[i].position.x = lArray[i - 1].position.x + 1;
//			break;
//		}
//
//		Text_Move(&lArray[i].projectileSprite, lArray[i].position.x, lArray[i].position.y);
//	}
//
//	lCount += laserSpeed;
//}

//void _ClearLaser()
//{
//	for (int i = 0; i < LENGTH_OF_LASER; i++)
//		lArray[i].active = false;
//
//	lCount = 0;
//}
//
//void _CheckProjectileBoundary()
//{
//	//Collision of Projectile to Boundary
//	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
//	{
//		if (pArray[i].position.x < Map_GetOrigin().x ||
//			pArray[i].position.y < Map_GetOrigin().y ||
//			pArray[i].position.x > Map_GetEnd().x ||
//			pArray[i].position.y > Map_GetEnd().y)
//		{
//			//Hide away projectiles
//			pArray[i].active = false;
//			return;
//		}
//	}
//}
//
//void _CheckLaserBoundary()
//{
//	if (lCount < 1)
//		return;
//
//	if (lArray[lCount - 1].position.x > GAME_WIDTH - MAP_OFFSET || lArray[lCount - 1].position.y > GAME_HEIGHT - MAP_OFFSET ||
//		lArray[lCount - 1].position.x < MAP_OFFSET || lArray[lCount - 1].position.y < MAP_OFFSET ||
//		lArray[lCount - 1].distanceToTravel < 0)
//	{
//		_ClearLaser();
//	}
//}
//
//void _CheckPlayerCollisionWithEnemy()
//{
//	// Player collision 
//	if (Player_GetState() == Dash)
//	{
//		for (int j = 0; j < player->playerSprite.charCount; j++)
//		{
//			for (int i = 0; i < enemy->enemySprite.charCount; i++)
//			{
//				if (enemy->enemySprite.origin.x + enemy->enemySprite.position[i].x == player->playerSprite.position[j].x + player->playerSprite.origin.x &&
//					enemy->enemySprite.origin.y + enemy->enemySprite.position[i].y == player->playerSprite.position[j].y + player->playerSprite.origin.y)
//				{
//					Player_ExtendDash();
//					// Add score
//					break;
//				}
//			}
//		}
//	}
//	else if (Player_GetState() == Normal)
//	{
//		// Enemy itself
//		for (int j = 0; j < player->playerSprite.charCount; j++)
//		{
//			for (int i = 0; i < enemy->enemySprite.charCount; i++)
//			{
//				if (enemy->enemySprite.origin.x + enemy->enemySprite.position[i].x == player->playerSprite.position[j].x + player->playerSprite.origin.x &&
//					enemy->enemySprite.origin.y + enemy->enemySprite.position[i].y == player->playerSprite.position[j].y + player->playerSprite.origin.y)
//				{
//					Player_Damage();
//					break;
//				}
//			}
//		}
//
//		//Projectile
//		for (int j = 0; j < player->playerSprite.charCount; j++)
//		{
//			// Projectile
//			for (int i = 1; i < NUMBER_OF_PROJECTILE; i++)
//			{
//				if (pArray[i - 1].position.x == player->playerSprite.position[j].x + player->playerSprite.origin.x &&
//					pArray[i - 1].position.y == player->playerSprite.position[j].y + player->playerSprite.origin.y)
//				{
//					//Hide away projectiles
//					pArray[i - 1].active = false;
//					Player_Damage();
//					break;
//				}
//			}
//		}
//
//		// Laser
//		for (int j = 0; j < player->playerSprite.charCount; j++)
//		{
//			for (int i = 0; i < lCount; i++)
//			{
//				if (lArray[i].position.x == player->playerSprite.position[j].x + player->playerSprite.origin.x &&
//					lArray[i].position.y == player->playerSprite.position[j].y + player->playerSprite.origin.y)
//				{
//					_ClearLaser();
//					Player_Damage();
//					break;
//				}
//			}
//		}
//	}
//}
