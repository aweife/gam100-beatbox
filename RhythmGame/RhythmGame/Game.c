#include "Game.h"

void Game_EnterState()
{
	// Call this to init player
	playerInit();
}

void Game_ExitState()
{

}

void Game_ProcessInput()
{
	EaseBool = 1;
	if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_UP))
	{
		playerSetDirection(TOPLEFT);
		EaseCheck = SpeedUp;
	}
		
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_UP))
	{
		playerSetDirection(TOPRIGHT);
		EaseCheck = SpeedUp;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_DOWN))
	{
		playerSetDirection(BOTTOMRIGHT);
		EaseCheck = SpeedUp;
	}
	else if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_DOWN))
	{
		playerSetDirection(BOTTOMLEFT);
		EaseCheck = SpeedUp;
	}
	else if (GetAsyncKeyState(VK_UP))
	{
		playerSetDirection(UP);
		EaseCheck = SpeedUp;
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		direction = RIGHT;
		EaseCheck = SpeedUp;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		playerSetDirection(DOWN);
		EaseCheck = SpeedUp;
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		playerSetDirection(LEFT);
		EaseCheck = SpeedUp;
	}
	else
	{
		if (playerGetDirection() != STAY)
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
	playerMove(direction);
	playerShape();
}

void Game_Render()
{
	gameRender();
}

/*************************************************************************************************************
											Player Functions
*************************************************************************************************************/


void gameRender()
{
	Player myPlayer = *playerGetPosition();
	for (int i = 0; i < 9; i++)
	{
		Console_SetRenderBuffer_CharColor(myPlayer.playerX[i], myPlayer.playerY[i], ' ', BACKGROUND_INTENSITY);
	}
}