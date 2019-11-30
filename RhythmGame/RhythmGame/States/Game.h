#pragma once

typedef enum GAMETYPE
{
	ONEPLAYER,
	TWOPLAYER,
	TUT,
} GAMETYPE;

extern void Game_EnterState(GAMETYPE type);
extern void Game_ExitState();
extern void Game_ProcessInput();
extern void Game_Update();
extern void Game_Render();
extern void Game_Exit();
extern int Game_IsRunning();

// For other files to know if its one player or two player game
extern GAMETYPE Game_GetGameType();