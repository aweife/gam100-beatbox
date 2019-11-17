#include "Enemy.h"
#include "../Attack/Attack.h"
#include "../Clock/Clock.h"
#include "../Global.h"
#include "../Map/Map.h"

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

void _spawnProjectile();
void _updateProjectile();
void _updateEnemy();

void Enemy_Init()
{
	// Initialise skull enemy
	skullEnemy = (Enemy) {.position.x = 50, .position.y = 20, enemySprite = Text_CreateSprite(),};
	Text_Init(&skullEnemy.enemySprite, "..//RhythmGame//$Resources//skull.txt");
}

void Enemy_FixedUpdate()
{
	_updateEnemy();
	Text_Move(&skullEnemy.enemySprite, skullEnemy.position.x, skullEnemy.position.y);

	// For testing only
	_spawnProjectile();
}

void Enemy_Render()
{
	//ASCI ENEMY
	Text_Render(&skullEnemy.enemySprite);

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
	
	Attack_spawnProjectile(skullEnemy.position, DOWN, 4);
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
