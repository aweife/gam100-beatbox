#pragma once

#define FRAMES 3

typedef enum MenuState
{
	PLAY = 0,
	PLAY2P = 1,
	LEVEL = 2,
	HISCORE = 3,
	CREDIT = 4,
	QUIT = 5,
	REPEAT = 6,
	CREDITSCREEN = 7,
	LEVEL_TUTORIAL = 8,
	LEVEL_LEVEL1 = 9,
};

extern void MainMenu_EnterState();
extern void MainMenu_ExitState();
extern void MainMenu_ProcessInput();
extern void MainMenu_Update();
extern void MainMenu_Render();
