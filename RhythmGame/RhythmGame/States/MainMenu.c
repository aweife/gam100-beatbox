#include <Windows.h>
#include "MainMenu.h"
#include "../Clock/Clock.h"
#include "../Text/TextReader.h"
#include "StateMachine.h"
#include "Game.h"
#include"../UI/UI.h"
#include "../Audio/AudioEngine.h"
#include "../Map/Map.h"

sprite diamond1;
sprite diamond2;
sprite diamond3;
static int choice = 1;
static int keyDown = 1;

static double beatTimer = 0.0;
static int beatFlag = 0;
static int toggle = 0;

static int spriteAni = 1;

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

	if (Audio_GetSpectrum(0) && toggle == 0)
	{
		beatFlag = 1;
		toggle = 1;
	}
	else
		toggle = 0;

	if (beatFlag == 1)
		_moveToBeat();
	
	_updateTimer();
	//Map_Update();
}


//*********************************************************************************
//									RENDER
//*********************************************************************************
void MainMenu_Render()
{
	//_renderChoice(choice);
	
	_renderBeat();

	// Render map
	//Map_Render();
}

//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void MainMenu_EnterState()
{
	diamond1 = Text_CreateSprite();
	Text_Init(&diamond1, "..//RhythmGame//$Resources//Diamond1.txt");
	diamond2 = Text_CreateSprite();
	Text_Init(&diamond2, "..//RhythmGame//$Resources//Diamond2.txt");
	diamond3 = Text_CreateSprite();
	Text_Init(&diamond3, "..//RhythmGame//$Resources//Diamond3.txt");

	Audio_Load(MAINMENU);
	Audio_PlayBGM(MAINMENU);
	//Map_Init();
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

void _moveToBeat()
{
	if (beatTimer < 0)
	{
		beatTimer = 15.0;
		spriteAni++;
		if (spriteAni >= 4)
		{
			spriteAni = 0;
			beatFlag = 0;
		}
	}
}

void _renderBeat()
{
	switch (spriteAni)
	{
	case 2:
		Text_Move(&diamond2, 23, 49);
		Text_Render(&diamond2, 0, 0);
		break;
	case 3:
		Text_Move(&diamond3, 21, 48);
		Text_Render(&diamond3, 0, 0);
		break;
	default:
		Text_Move(&diamond1, 25, 50);
		Text_Render(&diamond1, 0, 0);
		break;
	}
}

void _updateTimer()
{
	if (beatTimer >= 0.0)
		beatTimer -= Clock_GetDeltaTime();
}

void _renderChoice(int choice)
{
}