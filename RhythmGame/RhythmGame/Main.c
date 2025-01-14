/*******************************************************************************
* @filename Main.c
* @author(s) Ong Guan Hin
* @DP emails guanhin.ong@digipen.edu
* Brief Description:
* This file consist of the main code with statemachines to run the game properly.
* COPYRIGHT � 2019 BY DIGIPEN CORP, USA. ALL RIGHTS RESERVED.
*******************************************************************************/

#include "Global.h"
#include "Console/Console.h"
#include "States/StateMachine.h"
#include "States/Game.h"
#include "Clock/Clock.h"

#include "Audio/AudioEngine.h"
#include "Text/Font.h"
#include <stdio.h>

#define DEBUG_AUDIO 0

int main(void)
{
	Console_Init();
	Console_SetTitle("Beat Box");
	Console_SetCursorVisibility(0);

	// Create audio engine once
	Audio_Create();
	Font_Create();

	float timer = 10000.0f;

	if (DEBUG_AUDIO)
	{
		Console_SetWindowedMode(GAME_WIDTH, GAME_HEIGHT, true);
		Audio_Load(STAGEONE);
		Audio_PlayBGM(STAGEONE);
	}
	else
	{
		Console_SetSquareFont();
		Console_SetWindowedMode(GAME_WIDTH, GAME_HEIGHT, true);
		StateMachine_ChangeState(State_Logo);
	}

	// MAIN loop
	while (Game_IsRunning())
	{
		if (DEBUG_AUDIO)
		{
			Clock_GameLoopStart();
			Audio_Update();
			timer -= Clock_GetDeltaTime();
			if (timer < 0.0)
			{
				timer = 60000.0f;
				Audio_Unload(5);
				Audio_Load(MAINMENU);
				Audio_PlayBGM(MAINMENU);
			}

			printf("%d      %f\n", Audio_GetSpectrum(2), timer/1000);
		}
		else
		{
			Clock_GameLoopStart();
			StateMachine_Start();
			StateMachine_ProcessInput();
			StateMachine_Update();
			StateMachine_Render();
		}
	}

	// Cleanup
	Audio_Shutdown();
	Font_Cleanup();
	Console_CleanUp();

	return 0;
}