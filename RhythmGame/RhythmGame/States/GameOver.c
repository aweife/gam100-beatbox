#include "GameOver.h"
#include <Windows.h>
#include "StateMachine.h"
#include "Game.h"
#include "../Global.h"
#include "../Text/TextReader.h"
#include "../Console/Console.h"


//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************

sprite CryingBeatmanState1;

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
}


//*********************************************************************************
//									RENDER
//*********************************************************************************
void GameOver_Render()
{
	Text_Render(&CryingBeatmanState1, 0, 0);
}





//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void GameOver_EnterState()
{
	CryingBeatmanState1 = Text_CreateSprite();
	Text_Init(&CryingBeatmanState1, "..//RhythmGame//$Resources//CryingBeatman1.txt");
	Text_Move(&CryingBeatmanState1, (GAME_WIDTH / 4), (GAME_HEIGHT / 2));
}

void GameOver_ExitState()
{
}
