#include "Game.h"
#include <stdbool.h>
#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Audio/AudioEngine.h"
#include "../Enemy/Enemy.h"
#include "../Clock/Clock.h"
#include "../UI/UI.h"
#include "../Attack/Attack.h"
#include "../Random/Random.h"
#include "../UI/GameUI.h"
#include "../States/StateMachine.h"

#define DELAY_AFTER_GAME_ENDS 5000

static int reqExit = 0;

// For pressing spacebar once
int spaceDown = false;

// STAGE ONE
static double exitTimer = 0;
static bool stageOneEnded = 0;

/* Internal */
void _CheckGameEnd();

void Game_EnterState()
{
	// Random
	Random_Init();
	Player_Init();
	Map_Init();
	Enemy_Init();
	Attack_Init();
	GameUI_Init();

	// Play bgm for audio
	Audio_Load(STAGEONE);
	Audio_PlayBGM(STAGEONE);

	// For ending the game after song ends
	exitTimer = 0.0;
	stageOneEnded = false;
}

void Game_ExitState()
{
	Attack_Cleanup();
	Audio_Unload();
}

void Game_ProcessInput()
{
	// If game has not ended
	if (stageOneEnded) return;

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
	// If game has not ended
	if (stageOneEnded) return;

	Player_Update();
	Map_Update();
	GameUI_Update();
	Audio_Update();

	// Update together
	Enemy_Update();
	Attack_Update();

	_CheckGameEnd();
}

void Game_Render()
{
	// If game has not ended
	if (stageOneEnded) return;

	// By order of rendering
	GameUI_Render();
	Map_Render();
	Attack_Render();
	Enemy_Render();
	Player_Render();
}

void Game_Exit()
{
	reqExit = 1;
}

int Game_IsRunning()
{
	return (reqExit == 0);
}

void _CheckGameEnd()
{
	if (!Audio_GetSpectrum(4))
		exitTimer += Clock_GetDeltaTime();
	else
		exitTimer = 0.0;

	if (exitTimer >= DELAY_AFTER_GAME_ENDS)
	{
		StateMachine_ChangeState(State_MainMenu);
	}
}