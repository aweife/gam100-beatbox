#include "Enemy.h"
#include "../Attack/Attack.h"
#include "../Clock/Clock.h"
#include "../Global.h"
#include "../Map/Map.h"
#include "../Console/Console.h"

//Game Input
int randEnMove = 0;
double result = 0.0;

// The skull enemy
Enemy skullEnemy;

//Game Time
//double BPMEnTime = 0.0;
int BPMProjSpawnTime = 0;
//double BPMProjMoveTime = 0.0;
//double elapsedTimerTime = 0.0;
//char timeDisplay[10];

//Only stores 10 values
Projectile pArray[NUMBER_OF_PROJECTILE];

//Limits 10 bullets on-screen
int pCount = NUMBER_OF_PROJECTILE;

/* Internal functions */
void _updateEnemy();
void _spawnProjectile();
void _decideEnemyMove(int seed);

void Enemy_Init()
{
	// Initialise skull enemy
	skullEnemy = (Enemy) {.position.x = 45, .position.y = 45, .enemySprite = Text_CreateSprite(),};
	_decideEnemyMove(1);
	Text_Init(&skullEnemy.enemySprite, "..//RhythmGame//$Resources//skull.txt");
	Text_Move(&skullEnemy.enemySprite, skullEnemy.position.x, skullEnemy.position.y);
}

void Enemy_FixedUpdate()
{
	Enemy_MoveTo(0);
	Text_Move(&skullEnemy.enemySprite, skullEnemy.position.x, skullEnemy.position.y);

	// For testing only
	_spawnProjectile();
}

void Enemy_Render()
{
	//ASCI ENEMY
	Text_Render(&skullEnemy.enemySprite, 0, Map_GetShakeFactor(UP)/2);

	// Debug origin point
	Console_SetRenderBuffer_CharColor(skullEnemy.position.x, skullEnemy.position.y, '+', CYAN);

	//for (int i = 0; i < pCount; i++)
	//{
	//	if (pArray[i].visible)
	//	{
	//		//Print out projectile
	//		Console_SetRenderBuffer_Char(pArray[i].position.x, pArray[i].position.y, '*');
	//	}
	//}

	//LETTER ENEMY
	//Console_SetRenderBuffer_Char(EnX, EnY, 'E');
}

sprite *Enemy_GetEnemy()
{
	return &skullEnemy.enemySprite;
}

void _spawnProjectile()
{
	BPMProjSpawnTime++;
	if (BPMProjSpawnTime <= 3) return;
	BPMProjSpawnTime = 0;
	
	//Attack_SpawnProjectile(skullEnemy.position, DOWN, 5, 2000);
	Attack_SpawnLaser(skullEnemy.position, DOWN, 100);
}

void _updateEnemy()
{
		randEnMove = Random_Range(1, 6); //original is 4
		if (randEnMove == 1)
		{
			//LEFT
			skullEnemy.position.x -= 2;
		}
		else if (randEnMove == 2) {
			//RIGHT
			skullEnemy.position.x += 2;
		}
		else if (randEnMove == 3) {
			//UPskullEnemy.position.x
			//EnY -= 1;
			skullEnemy.position.x -= 4;
		}
		else if (randEnMove == 4) {
			//DOWN
			//EnY += 1;
			skullEnemy.position.x += 4;
		}
		else if (randEnMove == 5) { //just for the prototype
			skullEnemy.position.x -= 6;
		}
		else if (randEnMove == 6) {
			skullEnemy.position.x += 6;
		}
}

void Enemy_MoveTo(int seed)
{
	if (seed == 0)
		seed = Random_Range(1, 9);

	if (skullEnemy.position.x == skullEnemy.newPosition.x && skullEnemy.position.y == skullEnemy.newPosition.y)
		_decideEnemyMove(seed);
	else
	{
		if (skullEnemy.position.x != skullEnemy.newPosition.x)
			skullEnemy.position.x += (skullEnemy.position.x > skullEnemy.newPosition.x) ? -1 : +1;

		if (skullEnemy.position.y != skullEnemy.newPosition.y)
			skullEnemy.position.y += (skullEnemy.position.y > skullEnemy.newPosition.y) ? -1 : +1;
	}
}

void _decideEnemyMove(int seed)
{
	
	switch (seed)
	{
	case 1: 
		skullEnemy.newPosition.x = MAP_OFFSET + (GAME_WIDTH / 6);
		skullEnemy.newPosition.y = MAP_OFFSET + (GAME_HEIGHT / 6);
		break;
	case 2:
		skullEnemy.newPosition.x = MAP_OFFSET + (GAME_WIDTH / 6) * 3;
		skullEnemy.newPosition.y = MAP_OFFSET + (GAME_HEIGHT / 6);
		break;
	case 3:
		skullEnemy.newPosition.x = MAP_OFFSET + (GAME_WIDTH / 6) * 5;
		skullEnemy.newPosition.y = MAP_OFFSET + (GAME_HEIGHT / 6);
		break;
	case 4:
		skullEnemy.newPosition.x = MAP_OFFSET + (GAME_WIDTH / 6);
		skullEnemy.newPosition.y = MAP_OFFSET + (GAME_HEIGHT / 6) * 3;
		break;
	case 5:
		skullEnemy.newPosition.x = MAP_OFFSET + (GAME_WIDTH / 6) * 3;
		skullEnemy.newPosition.y = MAP_OFFSET + (GAME_HEIGHT / 6) * 3;
		break;
	case 6:
		skullEnemy.newPosition.x = MAP_OFFSET + (GAME_WIDTH / 6) * 5;
		skullEnemy.newPosition.y = MAP_OFFSET + (GAME_HEIGHT / 6) * 3;
		break;
	case 7:
		skullEnemy.newPosition.x = MAP_OFFSET + (GAME_WIDTH / 6);
		skullEnemy.newPosition.y = MAP_OFFSET + (GAME_HEIGHT / 6) * 5;
		break;
	case 8:
		skullEnemy.newPosition.x = MAP_OFFSET + (GAME_WIDTH / 6) * 3;
		skullEnemy.newPosition.y = MAP_OFFSET + (GAME_HEIGHT / 6) * 5;
		break;
	case 9:
		skullEnemy.newPosition.x = MAP_OFFSET + (GAME_WIDTH / 6) * 5;
		skullEnemy.newPosition.y = MAP_OFFSET + (GAME_HEIGHT / 6) * 5;
		break;
	}
}