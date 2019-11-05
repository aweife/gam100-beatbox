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
int hasitSpawn = 0;
int randNo = 0;
int tmpX = 0;
int tmpY = 0;

//Game Update
int direction = 0;
double velocity = 0.01f;

//Game Time
float BPMEnTime = 0.0f;
float BPMProjSpawnTime = 0.0f;
float BPMProjMoveTime = 0.0f;
float elapsedTimerTime = 0.0f;
char timeDisplay[10];

void Clock()
{
	elapsedTimerTime += Clock_GetDeltaTime();
	_itoa_s(elapsedTimerTime / CLOCKS_PER_SEC, timeDisplay, sizeof(timeDisplay), 10);
	Console_SetRenderBuffer_String(63, 8, "Timer:");
	Console_SetRenderBuffer_String(70, 8, timeDisplay);
}

void Boundary()
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

typedef struct //MUSTLEARNTHIS
{
	int x;
	int y;
	int state;
	int pArrayReady;
} Projectile;

Projectile pArray[5]; //Only stores 3 elements
int pCount = 5; //Limits 3 bullets on-screen

void ProjectileSpawn()
{
	BPMProjSpawnTime += Clock_GetDeltaTime();
	if (BPMProjSpawnTime >= 4000.0f) //It will ONLY spawn at 4s interval
	{
		for (int i = 0; i < pCount; ++i)
		{
			if (pArray[i].pArrayReady == 0 && pArray[i].state == 0) //State and Ready ensures array don't spawn unnecessary projectiles > 3
			{
				pArray[i].pArrayReady = 1; 
				BPMProjSpawnTime = BPMProjSpawnTime - 4000.0f; //reset spawn time
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
			Console_SetRenderBuffer_Char(pArray[i].x, pArray[i].y , '*'); //Print out projectile
		}

		if (pArray[i].x > width - 2 || pArray[i].x < 1 || pArray[i].y > width - 2 || pArray[i].y < 1) //Collision
		{
			pArray[i].state = 0;
			//Hide away projectiles
			pArray[i].x = 90; 
			pArray[i].y = 90;
		}
	}
}

void ProjectileMovement()
{
	BPMProjMoveTime += Clock_GetDeltaTime();
	if (BPMProjMoveTime >= 800.0f)
	{
		for (int i = 0; i < pCount; i++)
		{
			pArray[i].x++; //Projectile only moves to the left
		}
		BPMProjMoveTime = BPMProjMoveTime - 800.0f; //Resets enemy move time to move every 1 second
	}
}

void enemyCharacters()
{
	Console_SetRenderBuffer_Char(EnX, EnY, 'E');
}


void enemyState()
{
	BPMEnTime += Clock_GetDeltaTime();

	if (BPMEnTime >= 800.0f) //Every 0.8s interval
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
		BPMEnTime = BPMEnTime - 800.0f;
	}
}


//int main()
//{
//	//Game Initialization
//	Random_Init();
//	Console_Init();
//
//	//Setup a square font and the window size, and remove cursor visibility
//	Console_SetTitle("Enemy Gameplay");
//	Console_SetSquareFont();
//	Console_SetWindowedMode(120, 60, false);
//	Console_SetCursorVisibility(false);
//
//	randNo = Random_Range(1, 4);
//	
//	while (GameIsRunning = 1)
//	{
//		Console_ClearRenderBuffer();
//		Clock_GameLoopStart();
//		Clock();
//		Boundary();
//		ProjectileSpawn();
//		ProjectileMovement();
//		enemyCharacters();
//		enemyState();
//		/*if (hasitSpawn = 1)
//		{
//			ProjectileSpawn();
//			ProjectileMovement();
//		}*/
//		Console_SwapRenderBuffer();
//	}
//}

