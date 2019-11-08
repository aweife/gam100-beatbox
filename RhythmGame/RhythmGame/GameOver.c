#include "GameOver.h"
#include <Windows.h>
#include "Console/Console.h"
#include "StateMachine.h"
#include "Game.h"

//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************


//*********************************************************************************
//									INPUT
//*********************************************************************************
void GameOver_ProcessInput()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 1)
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
	Console_SetRenderBuffer_String(16, 16, " _____________________________________________________");
	Console_SetRenderBuffer_String(16, 17, "   _____                         ____                 ");
	Console_SetRenderBuffer_String(16, 18, "  / ____|                       / __ \\                ");
	Console_SetRenderBuffer_String(16, 19, " | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ ");
	Console_SetRenderBuffer_String(16, 20, " | | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|");
	Console_SetRenderBuffer_String(16, 21, " | |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |   ");
	Console_SetRenderBuffer_String(16, 22, "  \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|   ");
	Console_SetRenderBuffer_String(16, 23, " _____________________________________________________");
	Console_SetRenderBuffer_String(28, 30, "Press Esc to go back to Main Menu");
}





//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void GameOver_EnterState()
{
}

void GameOver_ExitState()
{
}
