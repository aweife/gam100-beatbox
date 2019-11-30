#include "GameOver.h"
#include <Windows.h>
#include "StateMachine.h"
#include "Game.h"
#include "../Global.h"
#include "../Text/TextReader.h"
#include "../Console/Console.h"
#include "../Clock/Clock.h"
#include <stdbool.h>


//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************

sprite CryingBeatmanState1;
sprite CryingBeatmanState2;

static double animationDuration = 0.0;
static bool animate = false;

//*********************************************************************************
//									INPUT
//*********************************************************************************
void GameOver_ProcessInput()
{
	if (GetAsyncKeyState(VK_ESCAPE))
		Game_Exit();
	if (GetAsyncKeyState(VK_SPACE))
		StateMachine_ChangeState(State_MainMenu);
}

//*********************************************************************************
//									UPDATE
//*********************************************************************************
void GameOver_Update()
{
	animationDuration += Clock_GetDeltaTime();

	if (animationDuration >= 0.0 && animationDuration <= 500.0)
	{
		animate = false;
	}
	else if (animationDuration >= 500.0 && animationDuration <= 1000.0)
	{
		animate = true;
		animationDuration -= 1000.0;
	}
}


//*********************************************************************************
//									RENDER
//*********************************************************************************
void GameOver_Render()
{
	if (animate == false)
	{
		Text_Render(&CryingBeatmanState1, 0, 0);
	}

	if (animate == true)
	{
		Text_Render(&CryingBeatmanState2, 0, 0);
	}
}





//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void GameOver_EnterState()
{
	CryingBeatmanState1 = Text_CreateSprite();
	Text_Init(&CryingBeatmanState1, "..//RhythmGame//$Resources//CryingBeatman1.txt");
	Text_Move(&CryingBeatmanState1, (GAME_WIDTH / 4), (GAME_HEIGHT / 2));

	CryingBeatmanState2 = Text_CreateSprite();
	Text_Init(&CryingBeatmanState2, "..//RhythmGame//$Resources//CryingBeatman2.txt");
	Text_Move(&CryingBeatmanState2, (GAME_WIDTH / 4), (GAME_HEIGHT / 2));
}

void GameOver_ExitState()
{
}
