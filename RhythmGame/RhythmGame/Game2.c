#include "Game2.h"
#include "Console/Console.h"
#include "Clock/Clock.h"
#include <Windows.h>
#include <Math.h>

double pPosX = 40.0;
double pPosY = 40.0;
int direction = 0;
double dt = 0.0;
double velocity = 0.012f;

void Game2_EnterState()
{

}

void Game2_ExitState()
{

}

void Game2_ProcessInput()
{
	if (GetAsyncKeyState(VK_LEFT))
		direction = 1;
	else if (GetAsyncKeyState(VK_RIGHT))
		direction = 2;
	else if (GetAsyncKeyState(VK_UP))
		direction = 3;
	else if (GetAsyncKeyState(VK_DOWN))
		direction = 4;
	else
		direction = 0;
}

void Game2_Update()
{
	dt = Clock_GetDeltaTime();
	playerMove();
}

void Game2_Render()
{
	Console_SetRenderBuffer_Char((int)round(pPosX), (int)round(pPosY), 'O');
}

/*************************************************************************************************************
											Player Functions
*************************************************************************************************************/
void playerMove()
{
	switch (direction)
	{
	case 1: pPosX += -1 * dt * velocity; break;
	case 2: pPosX += 1 * dt * velocity; break;
	case 3: pPosY += -1 * dt * velocity; break;
	case 4: pPosY += 1 * dt * velocity; break;
	default: break;
	}
}