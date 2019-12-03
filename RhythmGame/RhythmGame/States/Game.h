#pragma once

// To update other files what mode is this
typedef enum GAMETYPE {
	ONEPLAYER,
	TWOPLAYER,
	TUT,
} GAMETYPE;

// Initialise the game according to gametype
extern void Game_EnterState(GAMETYPE type);

// Cleanup the game
extern void Game_ExitState();

// Take input before updating the game
extern void Game_ProcessInput();

// Update the game before rendering
extern void Game_Update();

// Render last
extern void Game_Render();

// Close the program
extern void Game_Exit();

// To run the game loop
extern int Game_IsRunning();

// For other files to know if its one player or two player game
extern GAMETYPE Game_GetGameType();