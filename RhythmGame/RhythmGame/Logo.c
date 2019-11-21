#include <Windows.h>
#include "Logo.h"
#include "States/StateMachine.h"
#include "Clock/Clock.h"



static double dTimer;
static double stateTimer = 5000.0;
static double velocity = 0.04;
LogoStruct Logo;

extern void Logo_EnterState()
{
	Logo = (LogoStruct){ .position.x = 35, .position.y = 130, .position.eulerY = 130.0, .LogoSprite = Text_CreateSprite(), };
	Text_Init(&Logo.LogoSprite, "..//RhythmGame//$Resources//digipenlogo.txt");
}

extern void Logo_ExitState()
{

}

extern void Logo_ProcessInput()
{
	if (GetAsyncKeyState(VK_SPACE))
		StateMachine_ChangeState(State_MainMenu);
}

extern void Logo_Update()
{
	Clock_GameLoopStart();

	dTimer = Clock_GetDeltaTime();
	if (Logo.position.eulerY < 30)
		Logo.position.eulerY = 29.0;
	else
		Logo.position.eulerY -= velocity * dTimer;

	Logo.position.y = (int)Logo.position.eulerY;
	Text_Move(&Logo.LogoSprite, Logo.position.x, Logo.position.y);

	if (stateTimer < 0)
		StateMachine_ChangeState(State_MainMenu);
	else
		stateTimer -= Clock_GetDeltaTime();
}

extern void Logo_Render()
{
	Text_Render(&Logo.LogoSprite, 0, 0);
}