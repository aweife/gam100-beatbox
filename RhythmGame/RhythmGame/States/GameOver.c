#include "GameOver.h"
#include <Windows.h>
#include "StateMachine.h"
#include "Game.h"
#include "../Global.h"
#include "../Text/TextReader.h"
#include "../Console/Console.h"
#include "../Clock/Clock.h"
#include <stdbool.h>

#define CENTER_OFFSETX 20
#define CENTER_OFFSETY 10

//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************

//Family of Sprites
sprite CryingBeatmanState1;
sprite CryingBeatmanState2;
sprite ReaperState1;
sprite ReaperState2;
sprite GameOver;
sprite GetScore;
sprite GetName;
sprite Enter;

//Static Variables
static double animationDuration = 0.0;
static bool animate = false;
static bool RETURN_DOWN = false;

//*********************************************************************************
//									INPUT
//*********************************************************************************
void GameOver_ProcessInput()
{
	if (GetAsyncKeyState(VK_RETURN) && !RETURN_DOWN)
	{
		RETURN_DOWN = true;
		StateMachine_ChangeState(State_MainMenu);
	}
	else if (!GetAsyncKeyState(VK_RETURN))
	{
		RETURN_DOWN = false;
	}

	if (GetAsyncKeyState(VK_ESCAPE) && !RETURN_DOWN)
	{
		Game_Exit();
	}
	else if (!GetAsyncKeyState(VK_ESCAPE))
	{
		RETURN_DOWN = false;
	}
}

//*********************************************************************************
//									UPDATE
//*********************************************************************************
void GameOver_Update()
{
	animationDuration += Clock_GetDeltaTime();

	//Logic to Animate in between to Sprite States
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
	
	Text_Render(&GameOver, 0, 0);
	_RenderCryingBeatmanAnimation();
	_RenderReaperAnimation();
	Text_Render(&GetScore, 0, 0);
	Text_Render(&GetName, 0, 0);
	Text_Render(&Enter, 0, 0);
}

//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void GameOver_EnterState()
{
	CryingBeatmanState1 = Text_CreateSprite();
	Text_Init(&CryingBeatmanState1, "..//RhythmGame//$Resources//CryingBeatman1.txt");
	Text_Move(&CryingBeatmanState1, (GAME_WIDTH / 4) + CENTER_OFFSETX - 5, (GAME_HEIGHT / 2) - CENTER_OFFSETY - 5);

	CryingBeatmanState2 = Text_CreateSprite();
	Text_Init(&CryingBeatmanState2, "..//RhythmGame//$Resources//CryingBeatman2.txt");
	Text_Move(&CryingBeatmanState2, (GAME_WIDTH / 4) + CENTER_OFFSETX - 5, (GAME_HEIGHT / 2) - CENTER_OFFSETY - 5);

	ReaperState1 = Text_CreateSprite();
	Text_Init(&ReaperState1, "..//RhythmGame//$Resources//Reaper1.txt");
	Text_Move(&ReaperState1, (GAME_WIDTH / 2) - CENTER_OFFSETX - 2, (GAME_HEIGHT / 4) - CENTER_OFFSETY - 5);

	ReaperState2 = Text_CreateSprite();
	Text_Init(&ReaperState2, "..//RhythmGame//$Resources//Reaper2.txt");
	Text_Move(&ReaperState2, (GAME_WIDTH / 2) - CENTER_OFFSETX - 27, (GAME_HEIGHT / 4) - CENTER_OFFSETY - 20);

	GameOver = Text_CreateSprite();
	Text_Init(&GameOver, "..//RhythmGame//$Resources//GameOver.txt");
	Text_Move(&GameOver, (GAME_WIDTH / 2) - CENTER_OFFSETX - 55, (GAME_HEIGHT / 4) + CENTER_OFFSETY - 5);

	GetScore = Text_CreateSprite();
	Text_Init(&GetScore, "..//RhythmGame//$Resources//GetScore.txt");
	Text_Move(&GetScore, (GAME_WIDTH / 2) - CENTER_OFFSETX - 75, (GAME_HEIGHT / 4) + CENTER_OFFSETY + 42);

	GetName = Text_CreateSprite();
	Text_Init(&GetName, "..//RhythmGame//$Resources//GetName.txt");
	Text_Move(&GetName, (GAME_WIDTH / 2) - CENTER_OFFSETX - 59, (GAME_HEIGHT / 4) + CENTER_OFFSETY + 62);

	Enter = Text_CreateSprite();
	Text_Init(&Enter, "..//RhythmGame//$Resources//GameOver_Enter.txt");
	Text_Move(&Enter, (GAME_WIDTH / 2) - CENTER_OFFSETX - 32, (GAME_HEIGHT / 4) + CENTER_OFFSETY + 85);
}

void GameOver_ExitState()
{
}

//*********************************************************************************
//								INTERNAL FUNCTIONS
//*********************************************************************************

void _RenderCryingBeatmanAnimation()
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

void _RenderReaperAnimation()
{
	if (animate == false)
	{
		Text_Render(&ReaperState1, 0, 0);
	}

	if (animate == true)
	{
		Text_Render(&ReaperState2, 0, 0);
	}
}