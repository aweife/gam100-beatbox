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
sprite playButton;
sprite levelButton;
sprite quitButton;
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
	Text_Init(&diamond1, "..//RhythmGame//$Resources//MainMenu//Diamond1.txt");
	diamond2 = Text_CreateSprite();
	Text_Init(&diamond2, "..//RhythmGame//$Resources//MainMenu//Diamond2.txt");
	diamond3 = Text_CreateSprite();
	Text_Init(&diamond3, "..//RhythmGame//$Resources//MainMenu//Diamond3.txt");
	playButton = Text_CreateSprite();
	Text_Init(&playButton, "..//RhythmGame//$Resources//MainMenu//PlayButton.txt");
	levelButton = Text_CreateSprite();
	Text_Init(&levelButton, "..//RhythmGame//$Resources//MainMenu//LevelButton.txt");
	quitButton = Text_CreateSprite();
	Text_Init(&quitButton, "..//RhythmGame//$Resources//MainMenu//QuitButton.txt");
	leftArrow = Text_CreateSprite();
	Text_Init(&leftArrow, "..//RhythmGame//$Resources//MainMenu//LeftArrow.txt");
	rightArrow = Text_CreateSprite();
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
	case 1:
		spriteColor = DARKGREEN;
		break;
	case 2:
		spriteColor = DARKBLUE;
		break;
	case 3:
		spriteColor = GREEN;
		break;
	default:
		spriteColor = DARKMAGENTA;
		break;
	}
}

void _renderChoice(int choice)
{
	switch (choice)
	{
	case PLAY:
		Text_Move(&playButton, 61, 75);
		Text_Render(&playButton, 0, 0);
		break;
	case LEVEL:
		Text_Move(&levelButton, 61, 75);
		Text_Render(&levelButton, 0, 0);
		break;
	case QUIT:
		Text_Move(&quitButton, 61, 75);
		Text_Render(&quitButton, 0, 0);
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