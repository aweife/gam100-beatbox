#include "Tutorial.h"
#include "../Global.h"
#include "../Map/Map.h"
#include <Windows.h>
#include "../Text/TextReader.h"
#include "../States/StateMachine.h"
#include "../States/Game.h"
#include"../UI/UI.h"
#include "../Text/TextReader.h"
#include <stdbool.h>

#define TUTORIAL_MAP_OFFSET 60

sprite Intro1;
sprite Intro2;
int intro2 = 0;
bool RETURN_DOWN = true;


//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************


//*********************************************************************************
//									INPUT
//*********************************************************************************
void Tutorial_ProcessInput()
{
	if (GetAsyncKeyState(VK_RETURN) && !RETURN_DOWN)
	{
		RETURN_DOWN = true;
		// Everything from here onwards will execute once
		intro2 = 1;
	}
	else if (!GetAsyncKeyState(VK_RETURN)) {
		RETURN_DOWN = false;
	}
	
}

//*********************************************************************************
//									UPDATE
//*********************************************************************************
void Tutorial_Update()
{
}

//*********************************************************************************
//									RENDER
//*********************************************************************************
void Tutorial_Render()
{
	if (intro2 == 0)
	{
		Text_Render(&Intro1);
	}

	if (intro2 == 1)
	{
		Text_Render(&Intro2);
	}
}

//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void Tutorial_EnterState()
{
	Intro1 = Text_CreateSprite();
	Text_Init(&Intro1, "..//RhythmGame//$Resources//Intro1.txt");
	Text_Move(&Intro1, (GAME_WIDTH / 2) - TUTORIAL_MAP_OFFSET, (GAME_HEIGHT / 2) - (TUTORIAL_MAP_OFFSET / 2));

	Intro2 = Text_CreateSprite();
	Text_Init(&Intro2, "..//RhythmGame//$Resources//Intro2.txt");
	Text_Move(&Intro2, (GAME_WIDTH / 2) - TUTORIAL_MAP_OFFSET, (GAME_HEIGHT / 2) - (TUTORIAL_MAP_OFFSET / 2));
}

void Tutorial_ExitState()
{

}