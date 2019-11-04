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
double velocity = 0.04f;

double factor = 0.0;
double EaseTimer = 0.0;
int EaseBool = 0;
int EaseCheck = 1;

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

typedef enum EaseMovement
{
	SpeedUp = 0,
	SlowDown = 1,
	Default = 2,
}EaseMovement;

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
	EaseBool = 1;
	if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_UP))
	{
		direction = TOPLEFT;
		EaseCheck = SpeedUp;
	}
		
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_UP))
	{
		direction = TOPRIGHT;
		EaseCheck = SpeedUp;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_DOWN))
	{
		direction = BOTTOMRIGHT;
		EaseCheck = SpeedUp;
	}
	else if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_DOWN))
	{
		direction = BOTTOMLEFT;
		EaseCheck = SpeedUp;
	}
	else if (GetAsyncKeyState(VK_UP))
	{
		direction = UP;
		EaseCheck = SpeedUp;
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		direction = RIGHT;
		EaseCheck = SpeedUp;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		direction = DOWN;
		EaseCheck = SpeedUp;
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		direction = LEFT;
		EaseCheck = SpeedUp;
	}
	else
	{
		if (direction != STAY)
		{
			EaseCheck = SlowDown;
			if (factor <= 0)
				direction = STAY;
		}
	}
}

void Game_Update()
{
	dt = Clock_GetDeltaTime();
	EaseTimer += Clock_GetDeltaTime();
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
	if (EaseTimer >= 25.0)
	{
		if (factor <= 1.0 && EaseCheck == SpeedUp)
			factor += 0.1;
		else if (factor >= 0.0 && EaseCheck == SlowDown)
			factor -= 0.1;

		EaseTimer = 0.0;
	}

	switch (direction)
	{
	case 1:
		pPosX += -1 * dt * velocity * factor;
		pPosY += -1 * dt * velocity * factor;
		diagonalCheck();
		break;
	case 2:
		pPosX += 1 * dt * velocity * factor;
		pPosY += -1 * dt * velocity * factor;
		break;
	case 3: 
		pPosX += 1 * dt * velocity * factor;
		pPosY += 1 * dt * velocity * factor;
		break;
	case 4: 
		pPosX += -1 * dt * velocity * factor;
		pPosY += 1 * dt * velocity * factor;
		break;
	case 5: pPosY += -1 * dt * velocity * factor; break;
	case 6: pPosX += 1 * dt * velocity * factor; break;
	case 7: pPosY += 1 * dt * velocity * factor; break;
	case 8: pPosX += -1 * dt * velocity * factor; break;
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
		Console_SetRenderBuffer_CharColor(Player1.playerX[i], Player1.playerY[i], ' ', BACKGROUND_INTENSITY);
	}
}