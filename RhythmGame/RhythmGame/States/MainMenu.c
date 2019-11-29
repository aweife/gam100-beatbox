#include <Windows.h>
#include "MainMenu.h"
#include "../Clock/Clock.h"
#include "../Text/TextReader.h"
#include "StateMachine.h"
#include "Game.h"
#include "../Global.h"
#include"../UI/UI.h"
#include "../Audio/AudioEngine.h"
#include "../Map/Map.h"

sprite diamond1;
sprite diamond2;
sprite diamond3;
sprite playButton;
sprite levelButton;
sprite quitButton;

static int i = 0;

static int choice = PLAY;
static int keyDown = 1;

static double beatTimer = 0.0;
static int beatFlag = 0;
static int toggle = 0;

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

	if (GetAsyncKeyState(VK_SPACE))
		_confirmChoice(choice);

	if (GetAsyncKeyState(VK_RETURN))
		StateMachine_ChangeState(State_Tutorial);

	if (GetAsyncKeyState(VK_LEFT) && keyDown == 0)
		choice--;

	if (GetAsyncKeyState(VK_RIGHT) && keyDown == 0)
		choice++;

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
}


//*********************************************************************************
//									RENDER
//*********************************************************************************
void MainMenu_Render()
{
	_renderBeat();
	_renderChoice(choice);
	// Render map first!
	Map_Render();


	_renderChoice(choice);
	Text_Move(&robot, 25, 50);
	Text_Render(&robot, 0, 0);
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
		Text_Render(&diamond1, 0, 0);
		break;
	}
}

void _updateTimer()
{
	if (beatTimer >= 0.0)
		beatTimer -= Clock_GetDeltaTime();
}

void _colorSwitch()
{
	switch (spriteColorCount)
	{
	case 1:
		spriteChar = 'b';
		spriteColor = DARKGREEN;
		break;
	case 2:
		spriteChar = 'c';
		spriteColor = DARKBLUE;
		break;
	case 3:
		spriteChar = 'g';
		spriteColor = GREEN;
		break;
	default:
		spriteChar = 'M';
		spriteColor = DARKMAGENTA;
		break;
	}

	for (i = 0; i < SPRITE_SIZE; i++)
	{
		if (diamond1.printchar[i] != spriteColor && diamond1.printchar[i] != ' ')
		{
			diamond1.printchar[i] = spriteChar;
			diamond1.printColor[i] = spriteColor;
		}
	}
}

void _renderChoice(int choice)
{
	spriteColorCount = choice;
	_colorSwitch();

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