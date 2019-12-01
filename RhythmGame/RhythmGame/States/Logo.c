#include <Windows.h>
#include "Logo.h"
#include "StateMachine.h"
#include "../Clock/Clock.h"
#include "../Audio/AudioEngine.h"

static double dTimer;
static double stateTimer = 4000.0;
static double velocity = 0.04;
LogoStruct Logo;

extern void Logo_EnterState()
{
	Logo = (LogoStruct){ .position.x = 3, .position.y = 100, .position.eulerY = 100.0, .LogoSprite1 = Text_CreateSprite(), .LogoSprite2 = Text_CreateSprite()};
	Text_Init(&Logo.LogoSprite1, "..//RhythmGame//$Resources//Logo.txt");
	Text_Init(&Logo.LogoSprite2, "..//RhythmGame//$Resources//Credit//Credit2_COPYRIGHT.txt");
}

extern void Logo_ExitState()
{
	Text_Cleanup(&Logo.LogoSprite1);
	Text_Cleanup(&Logo.LogoSprite2);

	Audio_Load(MAINMENU);
	Audio_PlayBGM(MAINMENU);
}

extern void Logo_ProcessInput()
{
	if (GetAsyncKeyState(VK_SPACE))
		StateMachine_ChangeState(State_MainMenu);
}

extern void Logo_Update()
{
	dTimer = Clock_GetDeltaTime();
	if (Logo.position.eulerY < 30)
		Logo.position.eulerY = 29.0;
	else
		Logo.position.eulerY -= velocity * dTimer;

	Logo.position.y = (int)Logo.position.eulerY;
	Text_Move(&Logo.LogoSprite1, Logo.position.x, Logo.position.y);
	Text_Move(&Logo.LogoSprite2, Logo.position.x + 15, Logo.position.y + 30);

	if (stateTimer < 0)
		StateMachine_ChangeState(State_MainMenu);
	else
		stateTimer -= Clock_GetDeltaTime();
}

extern void Logo_Render()
{
	Text_Render(&Logo.LogoSprite1, 0, 0);
	Text_Render(&Logo.LogoSprite2, 0, 0);
}