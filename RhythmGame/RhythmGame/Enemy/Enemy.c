#include "Enemy.h"

//Game State
int GameIsRunning = 0;
int height = 1;
int width = 80;

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

//Game Time
double BPMEnTime = 0.0;
double BPMProjSpawnTime = 0.0;
double BPMProjMoveTime = 0.0;
double elapsedTimerTime = 0.0;
char timeDisplay[10];

//Skull Structure
skullenemy skull;

//Only stores 10 values
Projectile pArray[NUMBER_OF_PROJECTILE];
//Limits 10 bullets on-screen
int pCount = NUMBER_OF_PROJECTILE;

void E_Init()
{
	E_CalculateBPM(132);
	Text_Init();
	skull = Text_CreateEnemy();
}

void E_Update()
{
	/*_getClock();*/
	_updateEnemy();
	Text_Moveenemy(&skull, EnX - 9, EnY - 7);
	_spawnProjectile();
	_updateProjectile();
}

/*void _getClock()
{
	elapsedTimerTime += Clock_GetDeltaTime();
	_itoa_s(elapsedTimerTime / CLOCKS_PER_SEC, timeDisplay, sizeof(timeDisplay), 10);
	Console_SetRenderBuffer_String(63, 8, "Timer:");
	Console_SetRenderBuffer_String(70, 8, timeDisplay);
}*/

/*void _renderBoundary()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j += (width - 1))
		{
			Console_SetRenderBuffer_Char(j, i, '#');
		}

	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j += (width - 1))
		{
			Console_SetRenderBuffer_Char(i, j, '#');
		}
	}
}*/

double E_CalculateBPM(int x)
{
	result = 60.0 / (double)x;
	result *= 1000.0;
	return result;
}

skullenemy *E_GetEnemy()
{
	return &skull;
}

Projectile *E_GetProjectile()
{
	return pArray;
}

void _spawnProjectile()
{
	BPMProjSpawnTime += Clock_GetDeltaTime();
	//It will ONLY spawn at 4s interval
	if (BPMProjSpawnTime >= 4000.0)
	{
		for (int i = 0; i < pCount; ++i)
		{
			//State and Ready ensures array don't spawn unnecessary projectiles > 10
			if (pArray[i].pArrayReady == 0 && pArray[i].state == 0)
			{
				pArray[i].pArrayReady = 1;
				//Reset spawn time
				BPMProjSpawnTime = BPMProjSpawnTime - 3000.0;
				break;
			}
		}

	}

	for (int i = 0; i < pCount; ++i)
	{
		if (pArray[i].pArrayReady == 1 && pArray[i].state == 0)
		{
			//Sets projectile to spawn at last location of enemy
			pArray[i].state = 1;
			pArray[i].x = EnX;
			pArray[i].y = EnY;
			pArray[i].pArrayReady = 0;
		}

		//Collision
		if (pArray[i].x >= GAME_WIDTH - MAP_OFFSET ||
			pArray[i].y >= GAME_HEIGHT - MAP_OFFSET ||
			pArray[i].x < MAP_OFFSET ||
			pArray[i].y < MAP_OFFSET)
		{
			pArray[i].state = 0;
			//Hide away projectiles
			pArray[i].x = 300;
			pArray[i].y = 300;
		}
	}
}

void _updateProjectile()
{
	BPMProjMoveTime += Clock_GetDeltaTime();
	if (BPMProjMoveTime >= result)
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
			pArray[i].y += 2;
		}
		BPMProjMoveTime = BPMProjMoveTime - result; //Resets enemy move time to move every 1 second
	}
}

void E_Render()
{
	//ASCI ENEMY
	Text_RenderEnemy(&skull);

	for (int i = 0; i < pCount; i++)
	{
		if (pArray[i].state == 1)
		{
			//Print out projectile
			Console_SetRenderBuffer_Char(pArray[i].x, pArray[i].y, '*');
		}
	}

	//LETTER ENEMY
	//Console_SetRenderBuffer_Char(EnX, EnY, 'E');
}

void _updateEnemy()
{
	BPMEnTime += Clock_GetDeltaTime();

	//Every 0.8s interval
	if (BPMEnTime >= result)
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
		BPMEnTime = BPMEnTime - result;
	}

	if (EnX >= 80 || EnX <= 20) //Reset Position if too near the border
	{
		EnX = 50;
		EnY = 20;
	}
}

/*
int main()
{
	//Game Initialization
	Random_Init();
	Console_Init();

	//Setup a square font and the window size, and remove cursor visibility
	Console_SetTitle("Enemy Gameplay");
	Console_SetSquareFont();
	Console_SetWindowedMode(120, 60, false);
	Console_SetCursorVisibility(false);

	randNo = Random_Range(1, 4);

	printf_s("%.1f", calculateBPM(132));

	while (GameIsRunning = 1)
	{
		Console_ClearRenderBuffer();
		Clock_GameLoopStart();
		_getClock();
		_renderBoundary();
		_spawnProjectile();
		_updateProjectile();
		renderEnemy();
		_updateEnemy();
		Console_SwapRenderBuffer();
	}
}
*/
