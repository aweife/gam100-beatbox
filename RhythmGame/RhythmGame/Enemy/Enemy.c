#include "Enemy.h"
#include "../Clock/Clock.h"
#include "../Global.h"
#include "../Map/Map.h"

//Game Input
int EnX = 50;
int EnY = 20;
int ProjX = 1;
int ProjY = 1;
int randEnMove = 0;
int randEnShoot = 0;
int tmpX = 0;
int tmpY = 0;
double result = 0.0;

Enemy enemyArray[1];

//Game Time
double BPMEnTime = 0.0;
double BPMProjSpawnTime = 0.0;
double BPMProjMoveTime = 0.0;
double elapsedTimerTime = 0.0;
char timeDisplay[10];

//Skull Structure
sprite skull;

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
	Enemy_CalculateBPM(132);
	skull = Text_CreateSprite();
	Text_Init(&skull, "..//RhythmGame//$Resources//skull.txt");
}

void Enemy_FixedUpdate()
{
	_updateEnemy();
	Text_Moveenemy(&skull, EnX - 9, EnY - 7);
	_spawnProjectile();
	_updateProjectile();
}

double Enemy_CalculateBPM(int x)
{
	result = 60.0 / (double)x;
	result *= 1000.0;
	return result;
}

void Enemy_Render()
{
	//ASCI ENEMY
	Text_RenderEnemy(&skull);

	for (int i = 0; i < pCount; i++)
	{
		if (pArray[i].visible)
		{
			//Print out projectile
			Console_SetRenderBuffer_Char(pArray[i].position.x, pArray[i].position.y, '*');
		}
	}

	//LETTER ENEMY
	//Console_SetRenderBuffer_Char(EnX, EnY, 'E');
}

sprite *Enemy_GetEnemy()
{
	return &skull;
}

Projectile *Enemy_GetProjectile()
{
	return pArray;
}

void _spawnProjectile()
{

	for (int i = 0; i < pCount; ++i)
	{
		//State and Ready ensures array don't spawn unnecessary projectiles > 10
		if (!pArray[i].available && !pArray[i].visible)
		{
			pArray[i].available = true;
			//Reset spawn time
			BPMProjSpawnTime = BPMProjSpawnTime - 3000.0;
			break;
		}
	}



	for (int i = 0; i < pCount; ++i)
	{
		if (pArray[i].available && !pArray[i].visible)
		{
			//Sets projectile to spawn at last location of enemy
			pArray[i].visible = true;
			pArray[i].position.x = EnX;
			pArray[i].position.y = EnY;
			pArray[i].available = false;
		}

		//Collision
		if (pArray[i].position.x >= GAME_WIDTH - MAP_OFFSET ||
			pArray[i].position.y >= GAME_HEIGHT - MAP_OFFSET ||
			pArray[i].position.x < MAP_OFFSET ||
			pArray[i].position.y < MAP_OFFSET)
		{
			pArray[i].available = 0;
			//Hide away projectiles
			pArray[i].position.x = 300;
			pArray[i].position.y = 300;
		}
	}
}


void _updateProjectile()
{
		//randEnShoot = Random_Range(1, 4);
		for (int i = 0; i < pCount; i++)
		{
			/*if (randEnShoot == 1)
			{
				//Right
				pArray[i].x++;
			}
			else if (randEnShoot == 2) {
				//Left
				pArray[i].x--;
			}
			else if (randEnShoot == 3) {
				//Up
				pArray[i].y--;
			}
			else if (randEnShoot == 4) {
				//Down
				pArray[i].y++;
			}*/
			pArray[i].position.y += 2;
		}
}

void _updateEnemy()
{

		randEnMove = Random_Range(1, 6); //original is 4
		if (randEnMove == 1)
		{
			//LEFT
			EnX -= 2;
		}
		else if (randEnMove == 2) {
			//RIGHT
			EnX += 2;
		}
		else if (randEnMove == 3) {
			//UP
			//EnY -= 1;
			EnX -= 4;
		}
		else if (randEnMove == 4) {
			//DOWN
			//EnY += 1;
			EnX += 4;
		}
		else if (randEnMove == 5) { //just for the prototype
			EnX -= 6;
		}
		else if (randEnMove == 6) {
			EnX += 6;
		}
	

	if (EnX >= 80 || EnX <= 20) //Reset Position if too near the border
	{
		EnX = 50;
		EnY = 20;
	}
}
