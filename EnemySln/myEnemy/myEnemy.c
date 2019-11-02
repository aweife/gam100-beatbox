#include "Console/Console.h"
#include "Random/Random.h"
#include "Clock/Clock.h"
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

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
//double ENeulerX = 0.0;
//double ENeulerY = 0.0;
double velocity = 0.01f;

//Game Time
float BPMEnTime = 0.0f;
float BPMProjSpawnTime = 0.0f;
float BPMProjMoveTime = 0.0f;
char timeDisplay[10];

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

//void ProjectileSpawn()
//{
//	Console_SetRenderBuffer_Char(ProjX, ProjY, '*');
//}
//
//void ProjectileShoot()
//{
//
//}

//typedef struct
//{
//	int artArray[100];
//} ART;

typedef struct //MUSTLEARNTHIS
{
	int x;
	int y;
} Projectile;

Projectile pArray[100];
int pCount = 0;

void ProjectileSpawn()
{
	BPMProjSpawnTime += Clock_GetDeltaTime();
	if (BPMProjSpawnTime >= 4000.0f) //4s
	{
		BPMProjSpawnTime = 0;
		tmpX = EnX;
		tmpY = EnY;
		pArray[pCount].x = tmpX;
		pArray[pCount].y = tmpY;
		pCount++;
	}
	for (int i = 0; i < pCount; i++)
	Console_SetRenderBuffer_Char(pArray[i].x, pArray[i].y, '*');
}

void ProjectileMovement()
{
	BPMProjMoveTime += Clock_GetDeltaTime();
	if (BPMProjMoveTime >= 4800.0f)
	{
		for (int i = 0; i < pCount; i++)
			pArray[i].x++;
		BPMProjMoveTime = 4000.0f;
	}
}

void enemyCharacters()
{
	Console_SetRenderBuffer_Char(EnX, EnY, 'E');
}


void enemyState()
{
	BPMEnTime += Clock_GetDeltaTime();

	if (BPMEnTime >= 800.0f) //every 0.8s
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

		//switch (randNo)
		//{
		//case 1: EnX -= 1;
		//		break;
		//case 2: EnX += 1;
		//		//hasitSpawn = 1;
		//		tmpX = EnX;
		//		tmpY = EnY;
		//		ProjX = tmpX;
		//		ProjY = tmpY;
		//		hasitSpawn = 1;
		//		break;
		//case 3: EnY -= 1;
		//		break;
		//case 4: EnY += 1;
		//		break;
		//}

		BPMEnTime = 0.0f;
	}
}


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
	
	while (GameIsRunning = 1)
	{
		Console_ClearRenderBuffer();
		Clock_GameLoopStart();
		Boundary();
		enemyCharacters();
		enemyState();
		/*if (hasitSpawn = 1)
		{
			ProjectileSpawn();
			ProjectileMovement();
		}*/
		ProjectileSpawn();
		ProjectileMovement();
		Console_SwapRenderBuffer();
	}
}

