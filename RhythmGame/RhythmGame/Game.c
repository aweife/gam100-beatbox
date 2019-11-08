#include "Game.h"

int spaceDown = false;

void Game_EnterState()
{
	// Call this to init player
	p_playerInit();
	Map_Init();
	AE_Init();
	E_Init();
}

void Game_ExitState()
{

}

void Game_ProcessInput()
{
	if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_UP))
		_playerSetVel(TOPLEFT, SpeedUp);
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_UP))
		_playerSetVel(TOPRIGHT, SpeedUp);
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_DOWN))
		_playerSetVel(BOTTOMRIGHT, SpeedUp);
	else if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_DOWN))
		_playerSetVel(BOTTOMLEFT, SpeedUp);
	else if (GetAsyncKeyState(VK_UP))
		_playerSetVel(UP, SpeedUp);
	else if (GetAsyncKeyState(VK_RIGHT))
		_playerSetVel(RIGHT, SpeedUp);
	else if (GetAsyncKeyState(VK_DOWN))
		_playerSetVel(DOWN, SpeedUp);
	else if (GetAsyncKeyState(VK_LEFT))
		_playerSetVel(LEFT, SpeedUp);
	else
	{
		if (_playerGetDirection() != STAY)
		{
			_playerSetVel(_playerGetDirection(), SlowDown);
			if (_playerGetEaseFactor() <= 0)
				_playerSetVel(STAY, SlowDown);
		}
	}

	if (GetAsyncKeyState(VK_SPACE) && !spaceDown)
	{
		spaceDown = true;
		_playerDash();
	} else if (!GetAsyncKeyState(VK_SPACE)) spaceDown = false;
}

void Game_Update()
{
	P_Update();
	AE_Update();
	E_Update();
}

void Game_Render()
{
	E_Render();
	P_Render();
	Map_Render();
}
