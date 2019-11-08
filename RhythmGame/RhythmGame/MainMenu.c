#include "MainMenu.h"
#include <Windows.h>
#include "Console/Console.h"
#include "StateMachine.h"
#include "Game.h"

skullenemy Logo;

//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************


//*********************************************************************************
//									INPUT
//*********************************************************************************
void MainMenu_ProcessInput()
{
	if (GetAsyncKeyState(VK_ESCAPE))
		Global_Exit();
	if (GetAsyncKeyState(VK_SPACE))
		StateMachine_changeState(State_Game);
}

//*********************************************************************************
//									UPDATE
//*********************************************************************************
void MainMenu_Update()
{
	Text_Moveenemy(&Logo,50,20);
}


//*********************************************************************************
//									RENDER
//*********************************************************************************
void MainMenu_Render()
{
	Text_RenderEnemy(&Logo);
}





//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void MainMenu_EnterState()
{
	Text_Init();
	Logo = Text_CreateEnemy();
}

void MainMenu_ExitState()
{
}
