#include "MainMenu.h"
#include <Windows.h>
#include "../Text/TextReader.h"
#include "StateMachine.h"
#include "Game.h"
#include"../UI/UI.h"
#include "../Audio/AudioEngine.h"
#include "../Map/Map.h"

sprite logo;
sprite robot;
sprite eyeball;
sprite Eye_Play;
sprite Eye_Level;
sprite Eye_Quit;
int choice = 1;
int keyDown = 1;

//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************


//*********************************************************************************
//									INPUT
//*********************************************************************************
void MainMenu_ProcessInput()
{
	if (GetAsyncKeyState(VK_ESCAPE))
		Game_Exit();

	if (GetAsyncKeyState(VK_SPACE))
		_confirmChoice(choice);
	if (GetAsyncKeyState(VK_RETURN))
		StateMachine_ChangeState(State_Tutorial);


	if (GetAsyncKeyState(VK_LEFT) && keyDown == 0)
		choice--;

	if (GetAsyncKeyState(VK_RIGHT) && keyDown == 0)
		choice++;

	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(VK_LEFT))
		keyDown = 1;
	else
		keyDown = 0;
}

//*********************************************************************************
//									UPDATE
//*********************************************************************************
void MainMenu_Update()
{
	if (choice == 4)
		choice = 1;
	if (choice == 0)
		choice = 3;

	Map_Update();
}


//*********************************************************************************
//									RENDER
//*********************************************************************************
void MainMenu_Render()
{
	_renderChoice(choice);
	Text_Move(&robot, 25, 50);
	Text_Render(&robot, 0, 0);

	// Render map
	Map_Render();
}

//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void MainMenu_EnterState()
{
	robot = Text_CreateSprite();
	Text_Init(&robot, "..//RhythmGame//$Resources//Robot.txt");
	Eye_Play = Text_CreateSprite();
	Text_Init(&Eye_Play, "..//RhythmGame//$Resources//Eye_Play.txt");
	eyeball = Text_CreateSprite();
	Text_Init(&eyeball, "..//RhythmGame//$Resources//eyeball.txt");
	Eye_Level = Text_CreateSprite();
	Text_Init(&Eye_Level, "..//RhythmGame//$Resources//Eye_Level.txt");
	Eye_Quit = Text_CreateSprite();
	Text_Init(&Eye_Quit, "..//RhythmGame//$Resources//Eye_Quit.txt");

	Audio_Load(MAINMENU);
	Audio_PlayBGM(MAINMENU);
	Map_Init();
}

void MainMenu_ExitState()
{
	Audio_Unload();
}

void _confirmChoice(int choice)
{
	switch (choice)
	{
	case 1:
		StateMachine_ChangeState(State_Game);
		break;
	}
}

void _renderChoice(int choice)
{
	switch (choice)
	{
	case 1:
		Text_Move(&Eye_Play, 51, 71);
		Text_Render(&Eye_Play, 0, 0);
		Text_Move(&eyeball, 110, 78);
		Text_Render(&eyeball, 0, 0);
		break;
	case 2:
		Text_Move(&Eye_Level, 52, 72);
		Text_Render(&Eye_Level, 0, 0);
		Text_Move(&eyeball, 110, 73);
		Text_Render(&eyeball, 0, 0);
		break;
	case 3:
		Text_Move(&Eye_Quit, 50, 70);
		Text_Render(&Eye_Quit, 0, 0);
		Text_Move(&eyeball, 125, 75);
		Text_Render(&eyeball, 0, 0);
		break;
	}
}