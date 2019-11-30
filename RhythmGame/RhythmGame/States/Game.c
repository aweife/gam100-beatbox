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

// For pause
static bool gamePaused = 0;
int escDown, pKeyDown, enterDown;
sprite pauseButton;

/* Internal */
void _CheckGameEnd();
void _PauseGame(bool pause);

void Game_EnterState()
{
	// Random
	Random_Init();
	Map_Init();
	Enemy_Init();
	Attack_Init();

	// Special init
	Player_Init();
	GameUI_Init();

	// Play bgm for audio
	Audio_Load(STAGEONE);
	Audio_PlayBGM(STAGEONE);

	// For ending the game after song ends
	exitTimer = 0.0;
	stageOneEnded = false;

	// Set game unpaused
	pauseButton = Text_CreateSprite();
	Text_Init(&pauseButton, "..//RhythmGame//$Resources//pause.txt");
	Text_Move(&pauseButton, GAME_WIDTH / 2 - GAMEUI_OFFSET, MAP_OFFSET);
	_PauseGame(false);
}

void Game_ExitState()
{
	_PauseGame(false);
	Attack_Cleanup();
	Audio_Unload();
}

void Game_ProcessInput()
{
	// If game has not ended
	if (stageOneEnded) return;

	if (gamePaused)
	{
		// Press esc once to quit to main menu
		if (GetAsyncKeyState(VK_ESCAPE) && !escDown)
		{
			escDown = true;
			StateMachine_ChangeState(State_MainMenu);
		}
		else if (!GetAsyncKeyState(VK_ESCAPE)) escDown = false;

		// Press enter once to  resume
		if (GetAsyncKeyState(VK_RETURN) && !enterDown)
		{
			enterDown = true;
			_PauseGame(false);
		}
		else if (!GetAsyncKeyState(VK_RETURN)) enterDown = false;
		// Press P once to pause
		if (GetAsyncKeyState(0x50) && !pKeyDown)
		{
			pKeyDown = true;
			_PauseGame(false);
		}
		else if (!GetAsyncKeyState(0x50)) pKeyDown = false;
	}
	else // Play game
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

		// Press spacebar once to dash
		if (GetAsyncKeyState(VK_SPACE) && !spaceDown)
		{
			spaceDown = true;
			Player_Dash();
		}
		else if (!GetAsyncKeyState(VK_SPACE)) spaceDown = false;

		// Press esc once to pause
		if (GetAsyncKeyState(VK_ESCAPE) && !escDown)
		{
			escDown = true;
			_PauseGame(true);
		}
		else if (!GetAsyncKeyState(VK_ESCAPE)) escDown = false;
		// Press P once to pause
		if (GetAsyncKeyState(0x50) && !pKeyDown)
		{
			pKeyDown = true;
			_PauseGame(true);
		}
		else if (!GetAsyncKeyState(0x50)) pKeyDown = false;
	}
}

void Game_Update()
{
	// If game has not ended
	if (stageOneEnded) return;

	if (gamePaused)
	{

	}
	else
	{
		Player_Update();
		Map_Update();
		GameUI_Update();
		Audio_Update();

		// Update together
		Enemy_Update();
		Attack_Update();

		_CheckGameEnd();
	}
}

void Game_Render()
{
	// If game has not ended
	if (stageOneEnded) return;

	if (gamePaused)
		Text_RenderRainbow(&pauseButton, 0, 0);
	else
		GameUI_Render();

	// By order of rendering
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
		// TODO: CHANGE TO GAMEOVER WHEN DONE
		StateMachine_ChangeState(State_MainMenu);
	}
}

void _PauseGame(bool pause)
{
	gamePaused = pause;
	Audio_Pause(pause);
}