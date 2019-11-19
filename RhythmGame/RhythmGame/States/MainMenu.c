#include "MainMenu.h"
#include <Windows.h>
#include "../Text/TextReader.h"
#include "StateMachine.h"
#include "Game.h"
#include"../UI/UI.h"

sprite logo;

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
		StateMachine_ChangeState(State_Game);
}

//*********************************************************************************
//									UPDATE
//*********************************************************************************
void MainMenu_Update()
{
}


//*********************************************************************************
//									RENDER
//*********************************************************************************
void MainMenu_Render()
{
	Text_Move(&logo,70, 20);
	Text_Render(&logo,0,0);
	UI_Render();

}





//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void MainMenu_EnterState()
{
	logo = Text_CreateSprite();
	Text_Init(&logo, "..//RhythmGame//$Resources//logo2.txt");

	UI_Init();
	
}

void MainMenu_ExitState()
{
}
