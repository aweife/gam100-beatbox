#include "Attack.h"
#include "../Clock/Clock.h"
#include "../Global.h"
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../Console/Console.h"
#include "../Audio/AudioEngine.h"
#include "../States/Game.h"
#include <stdlib.h>
#include <math.h>

#define DEBUG_AABB 0
#define COLLISION_OFFSET 2

#define LASER_UPDATE_SPEED 4
#define NOTE_MOVESPEED 0.007

#define NOTE_SLOW_1 0.6
#define NOTE_SLOW_2 0.2
#define NOTE_SLOW_3 0.06
#define NOTE_SLOW_TIME 1500.0

// Keep tracks of how many projectiles are currently in use
// then we update all in-use projectiles
Projectile *pArray;
Projectile *plArray;
Laser *lArray;

// For notes
sprite notes[TYPES_OF_NOTES] = { 0 };
Note nArray[NUMBER_OF_NOTES] = { 0 };
static double noteTimer = 0;
static double slowFactor = 0;

/* Internal functions */
void _MoveProjectile();
void _CheckProjectileCollision();
void _MoveLaser();
void _CheckLaserCollision();
void _ClearLaser(int i);
void _MoveNote();
void _CheckNoteCollision();
void _SlowTime(double factor);

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

	// Init sprites for notes
	for (int i = 0; i < TYPES_OF_NOTES; i++)
	{
		notes[i] = Text_CreateSprite();
		Text_InitArray(&notes[i], "..//RhythmGame//$Resources//notes.txt", i);
	}

	// Init notes array
	for (int i = 0; i < NUMBER_OF_NOTES; i++)
	{
		nArray[i].active = false;
		nArray[i].startPosition = (Vector2d){ 0,0,0,0 };
		nArray[i].endPosition = (Vector2d){ 0,0,0,0 };
	}
	noteTimer = 0;
}

void Attack_Update()
{
	// Prevent projectile from moving past border
	_CheckProjectileCollision();
	_MoveProjectile();

	// Must update laser movement before collision! We want it to move past border
	_MoveLaser();
	_CheckLaserCollision();

	// Notes
	_MoveNote();
	_CheckNoteCollision();

	// Slow powerup
	if (noteTimer > 0.0)
		noteTimer -= Clock_GetDeltaTime();
	else if (noteTimer <= 0.0)
		slowFactor = 1.0;
}

void Attack_Render() // put in game.c
{
	//Print out projectile
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
		if (pArray[i].active)
		{
			CONSOLECOLOR c, d;
			if (noteTimer > 0.0)
			{
				c = CYAN;
				d = DARKCYAN;
			}
			else
			{
				c = WHITE;
				d = LIGHTGRAY;
			}
			// Change the letter color
			for (int j = 0, count = pArray[i].projectileSprite.charCount; j < count; j++)
				if (pArray[i].projectileSprite.spriteI[j].printchar == 'w')
					pArray[i].projectileSprite.spriteI[j].printColor = c;
				else if (pArray[i].projectileSprite.spriteI[j].printchar == 'd')
					pArray[i].projectileSprite.spriteI[j].printColor = d;

			Text_Render(&pArray[i].projectileSprite, 0, 0);
		}

	//Print out projectile
	for (int i = 0; i < NUMBER_OF_PLAYER_PROJECTILE; i++)
		if (plArray[i].active)
		{
			for (int j = 0; j < 3; j++)
				if (plArray[i].projectileSprite.spriteI[j].printchar == 'C')
					plArray[i].projectileSprite.spriteI[j].printColor = plArray[i].which ? DARKGREEN : DARKCYAN;
				else if (plArray[i].projectileSprite.spriteI[j].printchar == 'c')
					plArray[i].projectileSprite.spriteI[j].printColor = plArray[i].which ? GREEN : CYAN;

			Text_Render(&plArray[i].projectileSprite, 0, 0);
		}

	// Print out laser
	for (int i = 0; i < NUMBER_OF_LASER; i++)
	{
		// Laser body
		if (lArray[i].active)
			for (int j = 0; j < lArray[i].laserIndex; j++)
				for (int k = 0; k < 2; k++)
				{
					CONSOLECOLOR c, d;
					if (noteTimer > 0.0)
					{
						c = DARKCYAN;
						d = CYAN;
					}
					else
					{
						c = DARKRED;
						d = RED;
					}
					for (int m = 0, count = lArray[i].laserSprite[j][k].charCount; m < count; m++)
						if (lArray[i].laserSprite[j][k].spriteI[m].printchar == 'R')
							lArray[i].laserSprite[j][k].spriteI[m].printColor = c;
						else if (lArray[i].laserSprite[j][k].spriteI[m].printchar == 'r')
							lArray[i].laserSprite[j][k].spriteI[m].printColor = d;
					Text_Render(&lArray[i].laserSprite[j][k], 0, 0);
				}

		// Spawn should overlap laser body
		if (lArray[i].spawned)
		{
			CONSOLECOLOR c, d;
			if (noteTimer > 0.0)
			{
				c = CYAN;
				d = DARKCYAN;
			}
			else
			{
				c = DARKRED;
				d = RED;
			}
			for (int m = 0, count = lArray[i].spawnSprite.charCount; m < count; m++)
				if (lArray[i].spawnSprite.spriteI[m].printchar == 'R')
					lArray[i].spawnSprite.spriteI[m].printColor = c;
				else if (lArray[i].spawnSprite.spriteI[m].printchar == 'r')
					lArray[i].spawnSprite.spriteI[m].printColor = d;
			Text_Render(&lArray[i].spawnSprite, 0, 0);
		}

		// Debug 
		if (lArray[i].active && DEBUG_AABB)
		{
			Console_SetRenderBuffer_CharColor(lArray[i].startPosition.x, lArray[i].startPosition.y, '+', GREEN);
			Console_SetRenderBuffer_CharColor(lArray[i].startPositionCheck.x, lArray[i].startPositionCheck.y, '+', WHITE);
			Console_SetRenderBuffer_CharColor(lArray[i].endPosition.x, lArray[i].endPosition.y, '+', WHITE);
			Console_SetRenderBuffer_CharColor(lArray[i].endPositionCheck.x, lArray[i].endPositionCheck.y, '+', GREEN);
		}
	}

	// Print out notes
	for (int i = 0; i < NUMBER_OF_NOTES; i++)
	{
		if (nArray[i].active)
		{
			Text_Render(&nArray[i].noteSprite, 0, 0);
			if (DEBUG_AABB)
			{
				Console_SetRenderBuffer_CharColor(nArray[i].startPosition.x, nArray[i].startPosition.y, '+', RED);
				Console_SetRenderBuffer_CharColor(nArray[i].endPosition.x, nArray[i].endPosition.y, '+', WHITE);
			}
		}
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

void Attack_Spawn(ATTACKTYPE type, Vector2d spawnPosition, DIRECTION direction, projectileSpeed speed, int which)
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
			plArray[i].position.y -= which * 2;
			plArray[i].direction = direction;
			plArray[i].speed = speed;
			plArray[i].active = true;
			plArray[i].which = which;
			return;
		}
	case LASER:
		for (int i = 0; i < NUMBER_OF_LASER; i++)
		{
			// Find a laser that is not in use
			if (lArray[i].spawned || lArray[i].active) continue;
			Text_Move(&lArray[i].spawnSprite, spawnPosition.x, spawnPosition.y);
			lArray[i].startPosition = spawnPosition;
			lArray[i].endPosition = spawnPosition;
			lArray[i].direction = direction;
			lArray[i].laserIndex = 0;
			lArray[i].spawned = true;
			return;
		}
	}
}

void Attack_SpawnNote(Vector2d spawnPosition, SCORESTATE type)
{
	for (int i = 0; i < NUMBER_OF_NOTES; i++)
	{
		// Dont touch active notes
		if (nArray[i].active) continue;

		nArray[i].startPosition = spawnPosition;
		nArray[i].startPosition.eulerX = (double)nArray[i].startPosition.x;
		nArray[i].startPosition.eulerY = (double)nArray[i].startPosition.y;
		nArray[i].endPosition = spawnPosition;
		switch (type)
		{
		default:
			nArray[i].noteSprite = notes[0];
			break;
		case MEDIUM:
			nArray[i].noteSprite = notes[1];
			break;
		case BIG:
			nArray[i].noteSprite = notes[2];
			break;
		}
		nArray[i].noteType = type;
		nArray[i].active = true;
		return;
	}
}

void _MoveProjectile()
{
	// ENEMY PROJECTILE
	// Update their movement if they are active
	for (int i = 0; i < NUMBER_OF_PROJECTILE; i++)
	{
		// If projectile not in use, don't update it
		if (!pArray[i].active) continue;

		// Sync speed to projectile
		double speed = (Audio_GetSpectrum(1) ? pArray[i].speed.fast : pArray[i].speed.normal) * Clock_GetDeltaTime() * slowFactor;

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

	// PLAYER PROJECTILE
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

		// Check against player 
		for (int j = 0; j <= Game_GetGameType(); j++)
		{
			if (pArray[i].position.x >= Player_GetPlayer(j)->startPosition.x &&
				pArray[i].position.x <= Player_GetPlayer(j)->endPosition.x &&
				pArray[i].position.y >= Player_GetPlayer(j)->startPosition.y &&
				pArray[i].position.y <= Player_GetPlayer(j)->endPosition.y)
			{
				pArray[i].active = false;
				pArray[i].position.x = -3;
				pArray[i].position.y = -3;
				Player_Damage(j);
			}
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
		if (plArray[i].position.x > Enemy_GetEnemy()->startPosition.x &&
			plArray[i].position.x < Enemy_GetEnemy()->endPosition.x &&
			plArray[i].position.y > Enemy_GetEnemy()->startPosition.y &&
			plArray[i].position.y < Enemy_GetEnemy()->endPosition.y)
		{
			plArray[i].active = false;
			plArray[i].position.x = -3;
			plArray[i].position.y = -3;
			Enemy_Damage(plArray[i].which);
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
		lArray[i].startPositionCheck = lArray[i].startPosition;
		// Expand the laser according to the speed
		for (int j = 0; j < (int)ceil((double)LASER_UPDATE_SPEED * slowFactor); j++)
		{
			switch (lArray[i].direction)
			{
			case UP:
				lArray[i].endPosition.y--;
				lArray[i].endPositionCheck = lArray[i].endPosition;
				lArray[i].startPositionCheck.x += COLLISION_OFFSET;
				orientation = 0;
				break;
			case LEFT:
				lArray[i].endPosition.x--;
				lArray[i].endPositionCheck = lArray[i].endPosition;
				lArray[i].startPositionCheck.y += COLLISION_OFFSET;
				orientation = 1;
				break;
			case DOWN:
				lArray[i].endPosition.y++;
				lArray[i].endPositionCheck = lArray[i].endPosition;
				lArray[i].endPositionCheck.x += COLLISION_OFFSET;
				orientation = 0;
				break;
			case RIGHT:
				lArray[i].endPosition.x++;
				lArray[i].endPositionCheck = lArray[i].endPosition;
				lArray[i].endPositionCheck.y += COLLISION_OFFSET;
				orientation = 1;
				break;
			}

			Text_Move(&lArray[i].laserSprite[lArray[i].laserIndex][orientation], lArray[i].endPosition.x, lArray[i].endPosition.y);
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
		if (lArray[i].endPosition.x <= Map_GetOrigin().x ||
			lArray[i].endPosition.x >= Map_GetEnd().x ||
			lArray[i].endPosition.y <= Map_GetOrigin().y ||
			lArray[i].endPosition.y >= Map_GetEnd().y)
		{
			_ClearLaser(i);
		}

		// Check against player
		for (int j = 0; j <= Game_GetGameType(); j++)
		{
			if (lArray[i].direction == DOWN || lArray[i].direction == RIGHT)
			{
				if (lArray[i].startPosition.x < Player_GetPlayer(j)->endPosition.x &&
					lArray[i].endPositionCheck.x > Player_GetPlayer(j)->startPosition.x &&
					lArray[i].startPosition.y < Player_GetPlayer(j)->endPosition.y &&
					lArray[i].endPositionCheck.y > Player_GetPlayer(j)->startPosition.y)
				{
					if (Player_GetState(j) == NORMAL)
					{
						Player_Damage(j);
						_ClearLaser(i);
					}
				}
			}
			else if (lArray[i].direction == UP || lArray[i].direction == LEFT)
				if (lArray[i].endPosition.x < Player_GetPlayer(j)->endPosition.x &&
					lArray[i].startPositionCheck.x > Player_GetPlayer(j)->startPosition.x &&
					lArray[i].endPosition.y < Player_GetPlayer(j)->endPosition.y &&
					lArray[i].startPositionCheck.y > Player_GetPlayer(j)->startPosition.y)
				{
					if (Player_GetState(j) == NORMAL)
					{
						Player_Damage(j);
						_ClearLaser(i);
					}
				}
		}
	}
}

void _ClearLaser(int i)
{
	lArray[i].spawned = false;
	lArray[i].active = false;
	lArray[i].endPosition.x = -3;
	lArray[i].endPosition.y = -3;

	for (int j = 0; j < lArray[i].laserIndex; j++)
		for (int k = 0; k < 2; k++)
			Text_Move(&lArray[i].laserSprite[j][k], lArray[i].endPosition.x, lArray[i].endPosition.y);
}

void _MoveNote()
{
	// Move note
	for (int i = 0; i < NUMBER_OF_NOTES; i++)
	{
		// If projectile not in use, don't update it
		if (!nArray[i].active) continue;

		int targetX, targetY;

		// If two player, go to their center
		if (Game_GetGameType() == TWOPLAYER)
		{
			targetX = (Player_GetPlayer(0)->startPosition.x + Player_GetPlayer(1)->startPosition.x) / 2;
			targetY = (Player_GetPlayer(0)->startPosition.y + Player_GetPlayer(1)->startPosition.y) / 2;
		}
		else // Follow player
		{
			targetX = Player_GetPlayer(0)->startPosition.x;
			targetY = Player_GetPlayer(0)->startPosition.y;
		}

		// Follow player
		if (nArray[i].startPosition.x < targetX)
			nArray[i].startPosition.eulerX += NOTE_MOVESPEED * Clock_GetDeltaTime();
		if (nArray[i].startPosition.x > targetX)
			nArray[i].startPosition.eulerX -= NOTE_MOVESPEED * Clock_GetDeltaTime();
		if (nArray[i].startPosition.y < targetY)
			nArray[i].startPosition.eulerY += NOTE_MOVESPEED * Clock_GetDeltaTime();
		if (nArray[i].startPosition.y > targetY)
			nArray[i].startPosition.eulerY -= NOTE_MOVESPEED * Clock_GetDeltaTime();

		nArray[i].startPosition.x = (int)(nArray[i].startPosition.eulerX);
		nArray[i].startPosition.y = (int)(nArray[i].startPosition.eulerY);
		nArray[i].endPosition.x = (int)(nArray[i].startPosition.eulerX + nArray[i].noteSprite.spriteI[nArray[i].noteSprite.charCount - 1].position.x);
		nArray[i].endPosition.y = (int)(nArray[i].startPosition.eulerY + nArray[i].noteSprite.spriteI[nArray[i].noteSprite.charCount - 1].position.y);
		Text_Move(&nArray[i].noteSprite, nArray[i].startPosition.x, nArray[i].startPosition.y);
	}
}

void _CheckNoteCollision()
{
	for (int i = 0; i < NUMBER_OF_NOTES; i++)
	{
		if (!nArray[i].active) continue;

		for (int j = 0; j <= Game_GetGameType(); j++)
		{
			if (nArray[i].startPosition.x < Player_GetPlayer(j)->endPosition.x &&
				nArray[i].endPosition.x > Player_GetPlayer(j)->startPosition.x &&
				nArray[i].startPosition.y < Player_GetPlayer(j)->endPosition.y &&
				nArray[i].endPosition.y > Player_GetPlayer(j)->startPosition.y)
			{
				nArray[i].active = false;
				nArray[i].startPosition.x = -3;
				nArray[i].startPosition.y = -3;

				// Do time slow
				switch (nArray[i].noteType)
				{
				default:
					_SlowTime(NOTE_SLOW_1);
					break;
				case MEDIUM:
					_SlowTime(NOTE_SLOW_2);
					break;
				case BIG:
					_SlowTime(NOTE_SLOW_3);
					break;
				}

			}
		}
	}
}

void _SlowTime(double factor)
{
	noteTimer += NOTE_SLOW_TIME;
	if (slowFactor > factor)
		slowFactor = factor;
}
