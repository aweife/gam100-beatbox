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

#define PLAYERONE 0 // For index in array
#define PLAYERTWO 1

// Game state
static GAMETYPE gameType;
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
void _ProcessPlayerControls();

void Game_EnterState(GAMETYPE type)
{
	// Check if one player or two player
	gameType = type;

	// Usuals
	Random_Init();
	Map_Init();
	Enemy_Init();

	// Player specific
	Player_Init(type);
	Attack_Init();
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
		// Controls for game
		_ProcessPlayerControls();

		// Pause
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

	// Map shake, we dont want pause when the border is offset
	Map_Update();

	if (!gamePaused)
	{
		Player_Update(PLAYERONE);
		if (gameType == TWOPLAYER)
			Player_Update(PLAYERTWO);

		GameUI_Update();
		Audio_Update();

		// Update together
		Enemy_Update();
		Attack_Update();

		// To see if song ended
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
	Player_Render(PLAYERONE);
	if (gameType == TWOPLAYER)
		Player_Render(PLAYERTWO);
}

void Game_Exit()
{
	reqExit = 1;
}

int Game_IsRunning()
{
	return (reqExit == 0);
}

GAMETYPE Game_GetGameType()
{
	return gameType;
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

void _ProcessPlayerControls()
{
	// Press spacebar once to dash
	if (GetAsyncKeyState(VK_SPACE) && !spaceDown)
	{
		spaceDown = true;

		// If two player, WASD gets space for dash
		Player_Dash(gameType == TWOPLAYER);
	}
	else if (!GetAsyncKeyState(VK_SPACE)) spaceDown = false;

	// One player's controls
	if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_UP))
		Player_SetVel(TOPLEFT, SpeedUp, PLAYERONE);
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_UP))
		Player_SetVel(TOPRIGHT, SpeedUp, PLAYERONE);
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_DOWN))
		Player_SetVel(BOTTOMRIGHT, SpeedUp, PLAYERONE);
	else if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_DOWN))
		Player_SetVel(BOTTOMLEFT, SpeedUp, PLAYERONE);
	else if (GetAsyncKeyState(VK_UP))
		Player_SetVel(UP, SpeedUp, PLAYERONE);
	else if (GetAsyncKeyState(VK_RIGHT))
		Player_SetVel(RIGHT, SpeedUp, PLAYERONE);
	else if (GetAsyncKeyState(VK_DOWN))
		Player_SetVel(DOWN, SpeedUp, PLAYERONE);
	else if (GetAsyncKeyState(VK_LEFT))
		Player_SetVel(LEFT, SpeedUp, PLAYERONE);
	else
	{
		if (Player_GetDirection(PLAYERONE) != STAY)
		{
			Player_SetVel(Player_GetDirection(PLAYERONE), SlowDown, PLAYERONE);
			if (Player_GetEaseFactor(PLAYERONE) <= 0)
				Player_SetVel(STAY, SlowDown, PLAYERONE);
		}
	}

	// Two players' controls
	if (gameType == TWOPLAYER)
	{
		// Press enter once to dash for player one
		if (GetAsyncKeyState(VK_RETURN) && !enterDown)
		{
			enterDown = true;
			Player_Dash(PLAYERONE);
		}
		else if (!GetAsyncKeyState(VK_RETURN)) enterDown = false;

		// Player two controls
		// 0x41 = A, 0x44 = D, 0x53 = S, 0x57 = W
		// the arguement 1 is
		if (GetAsyncKeyState(0x41) && GetAsyncKeyState(0x57))
			Player_SetVel(TOPLEFT, SpeedUp, PLAYERTWO);
		else if (GetAsyncKeyState(0x44) && GetAsyncKeyState(0x57))
			Player_SetVel(TOPRIGHT, SpeedUp, PLAYERTWO);
		else if (GetAsyncKeyState(0x44) && GetAsyncKeyState(0x53))
			Player_SetVel(BOTTOMRIGHT, SpeedUp, PLAYERTWO);
		else if (GetAsyncKeyState(0x41) && GetAsyncKeyState(0x53))
			Player_SetVel(BOTTOMLEFT, SpeedUp, PLAYERTWO);
		else if (GetAsyncKeyState(0x57))
			Player_SetVel(UP, SpeedUp, PLAYERTWO);
		else if (GetAsyncKeyState(0x44))
			Player_SetVel(RIGHT, SpeedUp, PLAYERTWO);
		else if (GetAsyncKeyState(0x53))
			Player_SetVel(DOWN, SpeedUp, PLAYERTWO);
		else if (GetAsyncKeyState(0x41))
			Player_SetVel(LEFT, SpeedUp, PLAYERTWO);
		else
		{
			if (Player_GetDirection(PLAYERTWO) != STAY)
			{
				Player_SetVel(Player_GetDirection(PLAYERTWO), SlowDown, PLAYERTWO);
				if (Player_GetEaseFactor(PLAYERTWO) <= 0.0)
					Player_SetVel(STAY, SlowDown, PLAYERTWO);
			}
		}
	}
}