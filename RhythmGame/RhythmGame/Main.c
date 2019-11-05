#include "StateMachine.h"
#include "Functions.h"
#include "Console/Console.h"
#include "Clock/Clock.h"

int main(void)
{
	// Game Init
	Console_Init();
	Console_SetTitle("FikrulEatPig");
	Console_SetCursorVisibility(0);
	Console_SetSquareFont();
	Console_SetWindowedMode(240, 125, 0);
	StateMachine_changeState(State_Game);

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