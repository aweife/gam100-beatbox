#include <Windows.h>
#include "MainMenu.h"
#include "../Clock/Clock.h"
#include "../Text/TextReader.h"
#include "StateMachine.h"
#include "Game.h"
#include"../UI/UI.h"
#include "../Audio/AudioEngine.h"
#include "../Map/Map.h"

sprite diamond1;
sprite diamond2;
sprite diamond3;
sprite Button_PLAY;
sprite Button_2P;
sprite Button_LEVEL;
sprite Button_HISCORE;
sprite Button_CREDIT;
sprite Button_QUIT;
sprite leftArrow;
sprite rightArrow;

static int i = 0;

static int choice = PLAY;
static int keyDown = 1;

static double beatTimer = 0.0;
static int beatFlag = 0;
static int toggle = 0;

static double arrowTimer = 0.0;
static int shakeDirection = 0;

static int spriteAni = 0;
static int spriteColorCount = 1;
static char spriteChar = 0;
static int spriteColor = 0;

//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************


//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void MainMenu_EnterState()
{
	diamond1 = Text_CreateSprite();
	diamond2 = Text_CreateSprite();
	diamond3 = Text_CreateSprite();
	Text_Init(&diamond1, "..//RhythmGame//$Resources//MainMenu//Diamond1.txt");
	Text_Init(&diamond2, "..//RhythmGame//$Resources//MainMenu//Diamond2.txt");
	Text_Init(&diamond3, "..//RhythmGame//$Resources//MainMenu//Diamond3.txt");

	Button_PLAY = Text_CreateSprite();
	Button_2P = Text_CreateSprite();
	Button_LEVEL = Text_CreateSprite();
	Button_HISCORE = Text_CreateSprite();
	Button_CREDIT = Text_CreateSprite();
	Button_QUIT = Text_CreateSprite();	
	Text_Init(&Button_PLAY, "..//RhythmGame//$Resources//MainMenu//Button_PLAY.txt");
	Text_Init(&Button_2P, "..//RhythmGame//$Resources//MainMenu//Button_2P.txt");
	Text_Init(&Button_LEVEL, "..//RhythmGame//$Resources//MainMenu//Button_LEVEL.txt");
	Text_Init(&Button_HISCORE, "..//RhythmGame//$Resources//MainMenu//Button_HISCORE.txt");
	Text_Init(&Button_CREDIT, "..//RhythmGame//$Resources//MainMenu//Button_CREDIT.txt");
	Text_Init(&Button_QUIT, "..//RhythmGame//$Resources//MainMenu//Button_QUIT.txt");

	leftArrow = Text_CreateSprite();
	rightArrow = Text_CreateSprite();
	Text_Init(&leftArrow, "..//RhythmGame//$Resources//MainMenu//LeftArrow.txt");
	Text_Init(&rightArrow, "..//RhythmGame//$Resources//MainMenu//RightArrow.txt");

	Text_Move(&leftArrow, 12, 75);
	Text_Move(&rightArrow, 154, 75);

	Audio_Load(MAINMENU);
	Audio_PlayBGM(MAINMENU);
}

void MainMenu_ExitState()
{
	Audio_Unload();
}

//*********************************************************************************
//									INPUT
//*********************************************************************************
void MainMenu_ProcessInput()
{
	if (GetAsyncKeyState(VK_ESCAPE))
		Game_Exit();

	if (GetAsyncKeyState(VK_RETURN))
		_confirmChoice(choice);

	if (GetAsyncKeyState(VK_LEFT) && keyDown == 0)
	{
		choice--;
		arrowTimer = 100.0;
		shakeDirection = LEFT;
	}

	if (GetAsyncKeyState(VK_RIGHT) && keyDown == 0)
	{
		choice++;
		arrowTimer = 100.0;
		shakeDirection = RIGHT;
	}

	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(VK_LEFT))
		keyDown = 1;
	else
		keyDown = 0;

	if (choice == REPEAT)
		choice = PLAY;
	if (choice < PLAY)
		choice = QUIT;
}

//*********************************************************************************
//									UPDATE
//*********************************************************************************
void MainMenu_Update()
{
	if (Audio_GetSpectrum(0) && toggle == 0)
	{
		beatFlag = 1;
		toggle = 1;
	}
	else
		toggle = 0;

	if (beatFlag == 1)
		_moveToBeat();

	_updateTimer();

	spriteColorCount = choice;
	_colorSwitch();
}


//*********************************************************************************
//									RENDER
//*********************************************************************************
void MainMenu_Render()
{
	_renderArrow();
	_renderBeat();
	_renderChoice(choice);
	
}

//*********************************************************************************
//									FUNCTIONS
//*********************************************************************************
void _confirmChoice(int choice)
{
	switch (choice)
	{
	case 1:
		StateMachine_ChangeState(State_Game);
		break;
	}
}

void _moveToBeat()
{
	if (beatTimer < 0)
	{
		beatTimer = 5.0;
		spriteAni++;
		if (spriteAni > FRAMES)
		{
			spriteColorCount++;
			spriteAni = 0;
			beatFlag = 0;
		}
	}
}

void _renderBeat()
{
	switch (spriteAni)
	{
	case 2:
		Text_Move(&diamond2, 23, 49);
		Text_Render(&diamond2, 0, 0);
		break;
	case 3:
		Text_Move(&diamond3, 21, 48);
		Text_Render(&diamond3, 0, 0);
		break;
	default:
		Text_Move(&diamond1, 25, 50);
		Text_RenderColor(&diamond1, spriteColor, 0, 0);
		break;
	}
}

void _updateTimer()
{
	if (beatTimer >= 0.0)
		beatTimer -= Clock_GetDeltaTime();

	if (arrowTimer >= 0.0)
		arrowTimer -= Clock_GetDeltaTime();
}

void _colorSwitch()
{
	switch (spriteColorCount)
	{
	case PLAY:
		spriteColor = DARKMAGENTA; // M
		break;
	case PLAY2P:
		spriteColor = DARKCYAN; // C
		break;
	case LEVEL:
		spriteColor = DARKGREEN; // G
		break;
	case HISCORE:
		spriteColor = DARKGRAY; // D
		break;
	case CREDIT:
		spriteColor = DARKYELLOW; // Y
		break;
	case QUIT:
		spriteColor = DARKBLUE; // B
		break;
	}
}

void _renderChoice(int choice)
{
	switch (choice)
	{
	case PLAY:
		Text_Move(&Button_PLAY, 61, 75);
		Text_Render(&Button_PLAY, 0, 0);
		break;
	case PLAY2P:
		Text_Move(&Button_2P, 61, 75);
		Text_Render(&Button_2P, 0, 0);
		break;
	case LEVEL:
		Text_Move(&Button_LEVEL, 61, 75);
		Text_Render(&Button_LEVEL, 0, 0);
		break;
	case HISCORE:
		Text_Move(&Button_HISCORE, 61, 75);
		Text_Render(&Button_HISCORE, 0, 0);
		break;
	case CREDIT:
		Text_Move(&Button_CREDIT, 61, 75);
		Text_Render(&Button_CREDIT, 0, 0);
		break;
	case QUIT:
		Text_Move(&Button_QUIT, 61, 75);
		Text_Render(&Button_QUIT, 0, 0);
		break;
	}
}

int leftArrowX = 0;
int rightArrowX = 0;

void _renderArrow()
{
	Text_RenderColor(&leftArrow, RED, leftArrowX, 0);
	Text_RenderColor(&rightArrow, RED, rightArrowX, 0);
	if (arrowTimer < 0)
	{
		shakeDirection = 0;
		rightArrowX = 0;
		leftArrowX = 0;
	}
	else if (arrowTimer >= 0)
	{
		if (shakeDirection == RIGHT)
			rightArrowX = 4;
		else if (shakeDirection == LEFT)
			leftArrowX = -4;
	}
}