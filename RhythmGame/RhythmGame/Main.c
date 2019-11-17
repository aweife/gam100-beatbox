#include "Global.h"
#include "Console/Console.h"
#include "States/StateMachine.h"
#include "States/Game.h"

int main(void)
{
	Console_Init();
	Console_SetTitle("Beat Box");
	Console_SetCursorVisibility(0);
	Console_SetSquareFont();
	Console_SetWindowedMode(GAME_WIDTH, GAME_HEIGHT, true);
	//Console_SetFullScreen();
	StateMachine_ChangeState(State_MainMenu);

	while (Game_IsRunning())
	{
		StateMachine_Start();
		StateMachine_ProcessInput();
		StateMachine_Update();
		StateMachine_Render();
	}

	Console_CleanUp();

	return 0;
}