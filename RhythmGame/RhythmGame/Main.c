#include "Global.h"
#include "Console/Console.h"
#include "States/StateMachine.h"
#include "States/Game.h"

//#include "Audio/AudioEngine.h"
//#include <stdio.h>
//#include "Clock/Clock.h"

int main(void)
{
	Console_Init();
	Console_SetTitle("Beat Box");
	Console_SetCursorVisibility(0);
	Console_SetSquareFont();
	Console_SetWindowedMode(GAME_WIDTH, GAME_HEIGHT, true);
	StateMachine_ChangeState(State_MainMenu);

	// Play the music for stage one
	/*Audio_Init();
	Audio_PlayBGMWithDelay(0.001, STAGEONE);
	Audio_SetBGMVolume(0, SNARE);
	Audio_SetBGMVolume(0.1, PROJECTILE);
	Audio_SetBGMVolume(0, WARNING);
	Audio_SetBGMVolume(0, LASER);
	Audio_SetBGMVolume(0, BGM);*/

	while (Game_IsRunning())
	{
		/*Clock_GameLoopStart();
		Audio_Update();
		printf("%.20f\n", Audio_GetFrequency(PROJECTILE));*/
		StateMachine_Start();
		StateMachine_ProcessInput();
		StateMachine_Update();
		StateMachine_Render();
	}

	Console_CleanUp();

	return 0;
}