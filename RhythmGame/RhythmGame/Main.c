#include "StateMachine.h"
#include "Global.h"
#include "Console/Console.h"
#include "Clock/Clock.h"

int main(void)
{
	// Game Init
	Console_Init();
	Console_SetTitle("Game");
	Console_SetCursorVisibility(0);
	Console_SetSquareFont();
	Console_SetWindowedMode(80, 80, 1);
	
	StateMachine_changeState(State_Game2);

	while (gameRunning())
	{
		Clock_GameLoopStart();
		StateMachine_Start();
		StateMachine_ProcessInput();
		StateMachine_Update();
		StateMachine_Render();
	}

	Console_CleanUp();

	return 0;
}