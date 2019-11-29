#include "Attack.h"
#include "../Clock/Clock.h"
#include "../Global.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../Console/Console.h"
#include "../Enemy/Enemy.h"
#include "../Audio/AudioEngine.h"
#include <stdlib.h>

#define LASER_UPDATE_SPEED 3

// Keep tracks of how many projectiles are currently in use
// then we update all in-use projectiles
Projectile *pArray;
Projectile *plArray;
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
	plArray = (Projectile *)malloc(sizeof(Projectile) * NUMBER_OF_PLAYER_PROJECTILE);

	// Init projectiles sprites
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
	{
		pArray[i].active = false;
		pArray[i].position.x = -1;
		pArray[i].position.y = -1;
		pArray[i].projectileSprite = Text_CreateSprite();
		Text_Init(&pArray[i].projectileSprite, "..//RhythmGame//$Resources//projectile.txt");
	}

	// Init player projectiles sprites
	for (int i = 0; i < NUMBER_OF_PLAYER_PROJECTILE; i++)
	{
		plArray[i].active = false;
		plArray[i].position.x = -1;
		plArray[i].position.y = -1;
		plArray[i].projectileSprite = Text_CreateSprite();
		Text_Init(&plArray[i].projectileSprite, "..//RhythmGame//$Resources//projectile2.txt");
	}

	// Init laser sprites
	for (int i = 0; i < NUMBER_OF_LASER; i++)
	{
		_ClearLaser(i);
		lArray[i].spawnSprite = Text_CreateSprite();
		Text_Init(&lArray[i].spawnSprite, "..//RhythmGame//$Resources//laserspawn.txt");

		// 2 laser sprite
		for (int k = 0; k < 2; k++)
			for (int j = 0; j < LENGTH_OF_LASER; j++)
			{
				lArray[i].laserSprite[j][k] = Text_CreateSprite();
				Text_InitArray(&lArray[i].laserSprite[j][k], "..//RhythmGame//$Resources//laser.txt", k);
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

	//Print out projectile
	for (int i = 0; i < NUMBER_OF_PLAYER_PROJECTILE; i++)
		if (plArray[i].active)
			Text_Render(&plArray[i].projectileSprite, 0, 0);

	// Print out laser
	for (int i = 0; i < NUMBER_OF_LASER; i++)
	{
		// Laser body
		if (lArray[i].active)
			for (int j = 0; j < lArray[i].laserIndex; j++)
				for (int k = 0; k < 2; k++)
					Text_Render(&lArray[i].laserSprite[j][k], 0, 0);

		// Spawn should overlap laser body
		if (lArray[i].spawned)
			Text_Render(&lArray[i].spawnSprite, 0, 0);
	}
}

void Attack_Cleanup()
{
	// Free up the sprites after use
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
		Text_Cleanup(&pArray[i].projectileSprite);

	for (int i = 0; i < NUMBER_OF_PLAYER_PROJECTILE; i++)
		Text_Cleanup(&plArray[i].projectileSprite);

	for (int i = 0; i < NUMBER_OF_LASER; i++)
	{
		Text_Cleanup(&lArray[i].spawnSprite);
		for (int k = 0; k < 2; k++)
			for (int j = 0; j < LENGTH_OF_LASER; j++)
				Text_Cleanup(&lArray[i].laserSprite[j][k]);

	}

	free(pArray);
	free(lArray);
	free(plArray);
}

void Attack_Spawn(ATTACKTYPE type, Vector2d spawnPosition, DIRECTION direction, projectileSpeed speed)
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
			pArray[i].speed = speed;
			pArray[i].active = true;
			return;
		}
	case PLAYER:
		for (int i = 0; i < NUMBER_OF_PLAYER_PROJECTILE; i++)
		{
			// Find a projectile that is not in use
			if (plArray[i].active) continue;
			plArray[i].position = spawnPosition;
			plArray[i].direction = direction;
			plArray[i].speed = speed;
			plArray[i].active = true;
			return;
		}
	case LASER:
		for (int i = 0; i < NUMBER_OF_LASER; i++)
		{
			// Find a laser that is not in use
			if (lArray[i].spawned || lArray[i].active) continue;
			Text_Move(&lArray[i].spawnSprite, spawnPosition.x, spawnPosition.y);
			lArray[i].laserPosition = spawnPosition;
			lArray[i].direction = direction;
			lArray[i].laserIndex = 0;
			lArray[i].spawned = true;
			return;
		}
	}
}

void _MoveProjectile()
{
	// Update their movement if they are active
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
	{
		// If projectile not in use, don't update it
		if (!pArray[i].active) continue;

		// Sync speed to projectile
		double speed = (Audio_GetSpectrum(1) ? pArray[i].speed.fast : pArray[i].speed.normal) * Clock_GetDeltaTime();

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

	// Update their movement if they are active
	for (int i = 0; i < NUMBER_OF_PLAYER_PROJECTILE; i++)
	{
		// If projectile not in use, don't update it
		if (!plArray[i].active) continue;

		// Sync speed to projectile
		double speed = plArray[i].speed.normal * Clock_GetDeltaTime();
		plArray[i].position.eulerY -= speed;

		// Move sprite
		plArray[i].position.x = (int)plArray[i].position.eulerX;
		plArray[i].position.y = (int)plArray[i].position.eulerY;
		Text_Move(&plArray[i].projectileSprite, plArray[i].position.x, plArray[i].position.y);
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
		//continue;
		// Check against player 
		for (int j = 0, count = Player_GetPlayerSprite()->charCount; j < count; j++)
			if (pArray[i].position.x == Player_GetPlayerSprite()->spriteI[j].position.x + Player_GetPlayerSprite()->origin.x &&
				pArray[i].position.y == Player_GetPlayerSprite()->spriteI[j].position.y + Player_GetPlayerSprite()->origin.y)
			{
				pArray[i].active = false;
				pArray[i].position.x = -3;
				pArray[i].position.y = -3;
				Player_Damage();
			}
	}

	for (int i = 0; i < NUMBER_OF_PLAYER_PROJECTILE; i++)
	{
		// If projectile not in use, don't update it
		if (!plArray[i].active) continue;

		// Check against border
		if (plArray[i].position.y <= Map_GetOrigin().y)
		{
			plArray[i].active = false;
			plArray[i].position.x = -3;
			plArray[i].position.y = -3;
		}

		// Check against enemy 
		for (int j = 0, count = Enemy_GetEnemySprite()->charCount; j < count; j++)
			if (plArray[i].position.x == Enemy_GetEnemySprite()->spriteI[j].position.x + Enemy_GetEnemySprite()->origin.x &&
				plArray[i].position.y == Enemy_GetEnemySprite()->spriteI[j].position.y + Enemy_GetEnemySprite()->origin.y)
			{
				plArray[i].active = false;
				plArray[i].position.x = -3;
				plArray[i].position.y = -3;
				Enemy_Damage();
			}
	}
}

void _MoveLaser()
{
	// Update if beat hits
	if (Audio_GetSpectrum(2))
	{
		for (int i = 0; i < NUMBER_OF_LASER; i++)
		{
			if (!lArray[i].spawned) continue;

			lArray[i].active = true;
		}
	}

	for (int i = 0; i < NUMBER_OF_LASER; i++)
	{
		// Update the active laser only
		if (!lArray[i].active || !lArray[i].spawned) continue;

		int orientation = 0; // 0 = vertical, 1 = horizontal
		// Expand the laser according to the speed
		for (int j = 0; j < LASER_UPDATE_SPEED; j++)
		{
			switch (lArray[i].direction)
			{
			case UP:
				lArray[i].laserPosition.y--;
				orientation = 0;
				break;
			case DOWN:
				lArray[i].laserPosition.y++;
				orientation = 0;
				break;
			case LEFT:
				lArray[i].laserPosition.x--;
				orientation = 1;
				break;
			case RIGHT:
				lArray[i].laserPosition.x++;
				orientation = 1;
				break;
			}
			Text_Move(&lArray[i].laserSprite[lArray[i].laserIndex][orientation], lArray[i].laserPosition.x, lArray[i].laserPosition.y);
			lArray[i].laserIndex++;
		}
	}
}

void _CheckLaserCollision()
{
	for (int i = 0; i < NUMBER_OF_LASER; i++)
	{
		// If not in use
		if (!lArray[i].active || !lArray[i].spawned) continue;

		// Check against border
		if (lArray[i].laserPosition.x <= Map_GetOrigin().x ||
			lArray[i].laserPosition.x >= Map_GetEnd().x ||
			lArray[i].laserPosition.y <= Map_GetOrigin().y ||
			lArray[i].laserPosition.y >= Map_GetEnd().y)
		{
			_ClearLaser(i);
		}
		//continue;
		// Check against player
		for (int k = 0; k < lArray[i].laserIndex; k++) // every laser sprite
			for (int l = 0; l < 2; l++) // animation state
				for (int j = 0, count = Player_GetPlayerSprite()->charCount; j < count; j++)
					for (int m = 0, count2 = lArray[i].laserSprite[k][l].charCount; m < count2; m++)
						if (lArray[i].laserSprite[k][l].spriteI[m].position.x + lArray[i].laserSprite[k][l].origin.x ==
							Player_GetPlayerSprite()->spriteI[j].position.x + Player_GetPlayerSprite()->origin.x &&
							lArray[i].laserSprite[k][l].spriteI[m].position.y + lArray[i].laserSprite[k][l].origin.y ==
							Player_GetPlayerSprite()->spriteI[j].position.y + Player_GetPlayerSprite()->origin.y)
						{
							_ClearLaser(i);
							Player_Damage();
						}
	}
}

void _ClearLaser(int i)
{
	lArray[i].spawned = false;
	lArray[i].active = false;
	lArray[i].laserPosition.x = -3;
	lArray[i].laserPosition.y = -3;

	for (int j = 0; j < LENGTH_OF_LASER; j++)
		for (int k = 0; k < 2; k++)
			Text_Move(&lArray[i].laserSprite[j][k], lArray[i].laserPosition.x, lArray[i].laserPosition.y);
}
