#pragma once

#define FRAMES 3

typedef enum MenuState
{
	PRESSENTER = -3,
	PLAY = 0,
	PLAY2P = 1,
	LEVEL = 2,
	HISCORE = 3,
	CREDIT = 4,
	QUIT = 5,
	REPEAT = 6,
	LEVEL_TUTORIAL = 10,
	LEVEL_LEVEL1 = 11,
	CREDITSCREEN1 = 20,
	CREDITSCREEN2 = 21,
	CREDITSCREEN3 = 22,
};

extern void MainMenu_EnterState();
extern void MainMenu_ExitState();
extern void MainMenu_ProcessInput();
extern void MainMenu_Update();
extern void MainMenu_Render();

extern void _creditExitState();
