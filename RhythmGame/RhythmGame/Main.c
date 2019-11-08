#include "StateMachine.h"
#include "Functions.h"

int main(void)
{
	// Game Init
	Console_Init();
	Console_SetTitle("BeatBox");
	Console_SetCursorVisibility(0);
	Console_SetSquareFont();
	Console_SetWindowedMode(GAME_WIDTH, GAME_HEIGHT, true);
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