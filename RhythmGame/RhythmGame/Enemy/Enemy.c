#include "Enemy.h"

//Game State
int GameIsRunning = 0;
int height = 60;
int width = 60;

//Game Input
int EnX = 30;
int EnY = 30;
int ProjX = 1;
int ProjY = 1;
int randNo = 0;
int tmpX = 0;
int tmpY = 0;
double result = 0.0;

//Game Time
double BPMEnTime = 0.0;
double BPMProjSpawnTime = 0.0;
double BPMProjMoveTime = 0.0;
double elapsedTimerTime = 0.0;
char timeDisplay[10];

//Projectile Structure
typedef struct //MUSTLEARNTHIS
{
	int x;
	int y;
	int state;
	int pArrayReady;
} Projectile;

//Only stores 5 elements
Projectile pArray[5]; 
//Limits 5 bullets on-screen
int pCount = 5; 

void _getClock()
{
	elapsedTimerTime += Clock_GetDeltaTime();
	_itoa_s(elapsedTimerTime / CLOCKS_PER_SEC, timeDisplay, sizeof(timeDisplay), 10);
	Console_SetRenderBuffer_String(63, 8, "Timer:");
	Console_SetRenderBuffer_String(70, 8, timeDisplay);
}

void _renderBoundary()
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
}

double calculateBPM(int x)
{
	result = 60.0 / (float)x;
	result *= 1000.0;
	return result;
}

void _spawnProjectile()
{
	BPMProjSpawnTime += Clock_GetDeltaTime();
	//It will ONLY spawn at 4s interval
	if (BPMProjSpawnTime >= 4000.0) 
	{
		for (int i = 0; i < pCount; ++i)
		{
			//State and Ready ensures array don't spawn unnecessary projectiles > 5
			if (pArray[i].pArrayReady == 0 && pArray[i].state == 0)
			{
				pArray[i].pArrayReady = 1; 
				//Reset spawn time
				BPMProjSpawnTime = BPMProjSpawnTime - 4000.0;
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

		if (pArray[i].state == 1)
		{
			//Print out projectile
			Console_SetRenderBuffer_Char(pArray[i].x, pArray[i].y , '*');
		}

		//Collision
		if (pArray[i].x > width - 2 || pArray[i].x < 1 || pArray[i].y > width - 2 || pArray[i].y < 1) 
		{
			pArray[i].state = 0;
			//Hide away projectiles
			pArray[i].x = 90; 
			pArray[i].y = 90;
		}
	}
}

void _updateProjectile()
{
	BPMProjMoveTime += Clock_GetDeltaTime();
	if (BPMProjMoveTime >= result)
	{
		for (int i = 0; i < pCount; i++)
		{
			pArray[i].x++; //Projectile only moves to the left
		}
		BPMProjMoveTime = BPMProjMoveTime - result; //Resets enemy move time to move every 1 second
	}
}

void renderEnemy()
{
	//Print Out Enemy
	Console_SetRenderBuffer_Char(EnX, EnY, 'E');
}


void _updateEnemy()
{
	BPMEnTime += Clock_GetDeltaTime();

	//Every 0.8s interval
	if (BPMEnTime >= result) 
	{
		randNo = Random_Range(1, 4);
		if (randNo == 1)
		{
			EnX -= 1;
		}
		else if (randNo == 2) {
			EnX += 1;
		}
		else if (randNo == 3) {
			EnY -= 1;
		}
		else if (randNo == 4) {
			EnY += 1;
		}
		BPMEnTime = BPMEnTime - result;
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
