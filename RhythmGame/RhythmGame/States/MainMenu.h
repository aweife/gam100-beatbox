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
};

extern void MainMenu_EnterState();
extern void MainMenu_ExitState();
extern void MainMenu_ProcessInput();
extern void MainMenu_Update();
extern void MainMenu_Render();

extern void _renderChoice(int choice);
extern void _confirmChoice(int choice);
extern void _moveToBeat();
extern void _updateTimer();
extern void _colorSwitch();
extern void _renderBeat();
extern void _renderArrow();