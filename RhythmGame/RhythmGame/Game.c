#include "Game.h"
#include "Console/Console.h"
#include "Clock/Clock.h"
#include <Windows.h>
#include <Math.h>

#define boxSize 3

double pPosX = 40.0;
double pPosY = 40.0;
int direction = 0;
double dt = 0.0;
double velocity = 0.012f;

// EASY OPTION HUEHUE

typedef enum DIRECTION
{
	TOPLEFT = 1,
	TOPRIGHT = 2,
	BOTTOMRIGHT = 3,
	BOTTOMLEFT = 4,
	UP = 5,
	RIGHT = 6,
	DOWN = 7,
	LEFT = 8,
	STAY = 0,
}Direction;

typedef struct Player
{
	int originX;
	int originY;
	int playerX[boxSize * boxSize];
	int playerY[boxSize * boxSize];
}Player;

struct Player Player1;

void Game_EnterState()
{

}

void Game_ExitState()
{

}

void Game_ProcessInput()
{
	if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_UP))
		direction = TOPLEFT;
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_UP))
		direction = TOPRIGHT;
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_DOWN))
		direction = BOTTOMRIGHT;
	else if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_DOWN))
		direction = BOTTOMLEFT;
	else if (GetAsyncKeyState(VK_UP))
		direction = UP;
	else if (GetAsyncKeyState(VK_RIGHT))
		direction = RIGHT;
	else if (GetAsyncKeyState(VK_DOWN))
		direction = DOWN;
	else if (GetAsyncKeyState(VK_LEFT))
		direction = LEFT;
	else
		direction = STAY;
}

void Game_Update()
{
	dt = Clock_GetDeltaTime();
	playerMove();
	Player1.originX = (int)round(pPosX);
	Player1.originY = (int)round(pPosY);
	playerShape();
}

void Game_Render()
{
	gameRender();
}

/*************************************************************************************************************
											Player Functions
*************************************************************************************************************/
void playerMove()
{
	switch (direction)
	{
	case 1:
		pPosX += -1 * dt * velocity;
		pPosY += -1 * dt * velocity;
		diagonalCheck();
		break;
	case 2:
		pPosX += 1 * dt * velocity;
		pPosY += -1 * dt * velocity;
		diagonalCheck();
		break;
	case 3: 
		pPosX += 1 * dt * velocity;
		pPosY += 1 * dt * velocity;
		diagonalCheck();
		break;
	case 4: 
		pPosX += -1 * dt * velocity;
		pPosY += 1 * dt * velocity;
		diagonalCheck();
		break;
	case 5: pPosY += -1 * dt * velocity; break;
	case 6: pPosX += 1 * dt * velocity; break;
	case 7: pPosY += 1 * dt * velocity; break;
	case 8: pPosX += -1 * dt * velocity; break;
	default: break;
	}
}

void diagonalCheck()
{
	if ((int)pPosX != round(pPosX))
	{
		if ((int)pPosY == round(pPosY))
		{
			pPosX = round(pPosX);
		}
	}
	else
	{
		if ((int)pPosY != round(pPosY))
		{
			pPosY = round(pPosY);
		}
	}
}

void playerShape()
{
	int localx = 0;
	int localy = 0;

	localy = Player1.originY--;
	localx = Player1.originX--;

	for (int i = 0; i < boxSize; i++)
	{
		for (int j = 0; j < boxSize; j++)
		{
			Player1.playerX[i * 3 + j] = localx + j;
			Player1.playerY[i * 3 + j] = localy + i;
		}
	}
}

void gameRender()
{
	for (int i = 0; i < 9; i++)
	{
		Console_SetRenderBuffer_Char(Player1.playerX[i], Player1.playerY[i], 'Y');
	}
}