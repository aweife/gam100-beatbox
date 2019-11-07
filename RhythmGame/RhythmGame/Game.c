#include "Game.h"

void Game_EnterState()
{
	// Call this to init player
	p_playerInit();
	Map_Init();
	AE_Init();
	AE_LoadTrack("..//mysound.mp3", SFX);
	AE_StartBGMWithDelay(0, 5.0);
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
}

void Game_Update()
{
	p_playerMove();
	AE_Update();
	Map_SetOffset((int)AE_GetEnergy());
}

void Game_Render()
{
	p_Render();
	Map_Render();
}
