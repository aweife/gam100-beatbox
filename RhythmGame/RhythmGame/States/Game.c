#include "Game.h"
#include <stdbool.h>
#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Audio/AudioEngine.h"
#include "../Enemy/Enemy.h"
#include "../Clock/Clock.h"
#include "../Beat/Beat.h"
#include "../UI/UI.h"
#include "../Attack/Attack.h"
#include "../Random/Random.h"
#include "../UI/GameUI.h"

static int reqExit = 0;

void Game_EnterState()
{
	// Random
	Random_Init();

	// Call this to init player
	Player_Init();
	Map_Init();
	Audio_Init();
	Enemy_Init();
	Beat_Init();
	Attack_Init();
	GameUI_Init();

	// Play the music for stage one
	Audio_PlayBGMWithDelay(0.001, STAGEONE);
	Audio_SetBGMVolume(SILENCE_FACTOR, SNARE);
	Audio_SetBGMVolume(SILENCE_FACTOR, PROJECTILE);
	Audio_SetBGMVolume(SILENCE_FACTOR, WARNING);
	Audio_SetBGMVolume(SILENCE_FACTOR, LASER);
	Audio_SetBGMVolume(1.0, BGM);
}

void Game_ExitState()
{

}

int spaceDown = false;

void Game_ProcessInput()
{
	if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_UP))
		Player_SetVel(TOPLEFT, SpeedUp);
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_UP))
		Player_SetVel(TOPRIGHT, SpeedUp);
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_DOWN))
		Player_SetVel(BOTTOMRIGHT, SpeedUp);
	else if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_DOWN))
		Player_SetVel(BOTTOMLEFT, SpeedUp);
	else if (GetAsyncKeyState(VK_UP))
		Player_SetVel(UP, SpeedUp);
	else if (GetAsyncKeyState(VK_RIGHT))
		Player_SetVel(RIGHT, SpeedUp);
	else if (GetAsyncKeyState(VK_DOWN))
		Player_SetVel(DOWN, SpeedUp);
	else if (GetAsyncKeyState(VK_LEFT))
		Player_SetVel(LEFT, SpeedUp);
	else
	{
		if (Player_GetDirection() != STAY)
		{
			Player_SetVel(Player_GetDirection(), SlowDown);
			if (Player_GetEaseFactor() <= 0)
				Player_SetVel(STAY, SlowDown);
		}
	}

	if (GetAsyncKeyState(VK_SPACE) && !spaceDown)
	{
		spaceDown = true;
		Player_Dash();
	}
	else if (!GetAsyncKeyState(VK_SPACE)) spaceDown = false;
}

void Game_Update()
{
	Clock_GameLoopStart();

	Player_Update();
	Audio_Update();
	Map_Update();
	Attack_Update();
	Beat_Update();
	GameUI_Update();
}

void Game_Render()
{
	Enemy_Render();
	Player_Render();
	Map_Render();
	Attack_Render();
	GameUI_Render();
}

void Game_Exit()
{
	reqExit = 1;
}

int Game_IsRunning()
{
	return (reqExit == 0);
}