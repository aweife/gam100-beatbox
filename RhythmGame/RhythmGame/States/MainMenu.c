#include <Windows.h>
#include "MainMenu.h"
#include "../Clock/Clock.h"
#include "../Text/TextReader.h"
#include "StateMachine.h"
#include "Game.h"
#include"../UI/UI.h"
#include "../Audio/AudioEngine.h"
#include "../Map/Map.h"
#include "../Global.h"
#include "../Console/Console.h"

// MAIN MENU SPRITES
sprite diamond1, diamond2, diamond3;
sprite Button_PLAY;
sprite Button_2P;
sprite Button_LEVEL;
sprite Button_HISCORE;
sprite Button_CREDIT;
sprite Button_QUIT;
sprite Button_TUTORIAL;
sprite leftArrow, rightArrow;
sprite title;

// CREDIT SPRITES
sprite Role_GAMEPLAY;
sprite Role_AUDIO;
sprite Role_LEVEL;
sprite Name_FIKRUL;
sprite Name_NICO;
sprite Name_TED;
sprite Name_GUANHIN;

extern void _renderBackground(int offset, CONSOLECOLOR color);
extern void _renderChoice();
extern void _confirmChoice();
extern void _moveToBeat();
extern void _updateTimer();
extern void _colorSwitch();
extern void _renderBeat();
extern void _renderArrow();
extern void _renderTitle();

extern void _renderCredit();

static int i = 0;

static int choice = PLAY;
static int keyDown = 1;

static double beatTimer = 0.0;
static int beatFlag = 0;
static int titleFlag = 0;
static double toggleTimer1 = 0;
static double toggleTimer2 = 0;

static double arrowTimer = 0.0;
static int shakeDirection = 0;

static double titleTimer = 0.0;
static int titleShake = 4;

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
	Button_TUTORIAL = Text_CreateSprite();
	Text_Init(&Button_PLAY, "..//RhythmGame//$Resources//MainMenu//Button_PLAY.txt");
	Text_Init(&Button_2P, "..//RhythmGame//$Resources//MainMenu//Button_2P.txt");
	Text_Init(&Button_LEVEL, "..//RhythmGame//$Resources//MainMenu//Button_LEVEL.txt");
	Text_Init(&Button_HISCORE, "..//RhythmGame//$Resources//MainMenu//Button_HISCORE.txt");
	Text_Init(&Button_CREDIT, "..//RhythmGame//$Resources//MainMenu//Button_CREDIT.txt");
	Text_Init(&Button_QUIT, "..//RhythmGame//$Resources//MainMenu//Button_QUIT.txt");
	Text_Init(&Button_TUTORIAL, "..//RhythmGame//$Resources//MainMenu//Button_TUTORIAL.txt");

	leftArrow = Text_CreateSprite();
	rightArrow = Text_CreateSprite();
	Text_Init(&leftArrow, "..//RhythmGame//$Resources//MainMenu//Arrow_LEFT.txt");
	Text_Init(&rightArrow, "..//RhythmGame//$Resources//MainMenu//Arrow_RIGHT.txt");

	title = Text_CreateSprite();
	Text_Init(&title, "..//RhythmGame//$Resources//MainMenu//Title1.txt");

	// CREDIT SPRITES INIT
	Role_GAMEPLAY = Text_CreateSprite();
	Text_Init(&Role_GAMEPLAY, "..//RhythmGame//$Resources//Credit//Role_GAMEPLAY.txt");
	Role_AUDIO = Text_CreateSprite();
	Text_Init(&Role_AUDIO, "..//RhythmGame//$Resources//Credit//Role_AUDIO.txt");
	Role_LEVEL = Text_CreateSprite();
	Text_Init(&Role_LEVEL, "..//RhythmGame//$Resources//Credit//Role_LEVEL.txt");
	Name_FIKRUL = Text_CreateSprite();
	Text_Init(&Name_FIKRUL, "..//RhythmGame//$Resources//Credit//Name_FIKRUL.txt");
	Name_NICO = Text_CreateSprite();
	Text_Init(&Name_NICO, "..//RhythmGame//$Resources//Credit//Name_NICO.txt");
	Name_TED = Text_CreateSprite();
	Text_Init(&Name_TED, "..//RhythmGame//$Resources//Credit//Name_TED.txt");
	Name_GUANHIN = Text_CreateSprite();
	Text_Init(&Name_GUANHIN, "..//RhythmGame//$Resources//Credit//Name_GUANHIN.txt");

	// CREDIT SPRITES UPDATE
	Text_Move(&Role_GAMEPLAY, 10, 40);
	Text_Move(&Role_AUDIO, 10, 80);
	Text_Move(&Role_LEVEL, 10, 110);
	Text_Move(&Name_FIKRUL, 40, 55);
	Text_Move(&Name_NICO, 40, 65);
	Text_Move(&Name_TED, 40, 95);
	Text_Move(&Name_GUANHIN, 40, 125);

	Text_Move(&leftArrow, 12, 75);
	Text_Move(&rightArrow, 154, 75);
	Text_Move(&title, 27, 10);

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

	if (GetAsyncKeyState(VK_RETURN) && keyDown == 0)
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

	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_RETURN))
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
	if (Audio_GetSpectrum(0) && toggleTimer1 <= 0)
	{
		beatFlag = 1;
		toggleTimer1 = 200;
	}

	if ((Audio_GetSpectrum(1) || Audio_GetSpectrum(2)) && toggleTimer2 <= 0)
		titleFlag = 1;
	else
		titleFlag = 0;

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
	if (Audio_GetSpectrum(1))
		_renderBackground(2, fYELLOW);
	if (Audio_GetSpectrum(0))
		_renderBackground(4, fDARKRED);

	if (titleFlag == 1)
		_renderTitle();
	if (choice != CREDITSCREEN)
	{
		_renderArrow();
		_renderBeat();
		_renderChoice(choice);
	}
	else
		_renderCredit();
	
}

//*********************************************************************************
//									FUNCTIONS
//*********************************************************************************
void _confirmChoice()
{
	switch (choice)
	{
	case PLAY:
		StateMachine_ChangeMode(ONEPLAYER);
		StateMachine_ChangeState(State_Game);
		break;
	case PLAY2P:
		StateMachine_ChangeMode(TWOPLAYER);
		StateMachine_ChangeState(State_Game);
		break;
	case LEVEL:
		choice = LEVEL_TUTORIAL;
		break;
	case HISCORE:
		break;
	case CREDIT:
		choice = CREDITSCREEN;
		break;
	case LEVEL_TUTORIAL:
		StateMachine_ChangeMode(ONEPLAYER);
		StateMachine_ChangeState(State_Tutorial);
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

	if (titleTimer >= 0.0)
		titleTimer -= Clock_GetDeltaTime();

	if (toggleTimer1 >= 0.0)
		toggleTimer1 -= Clock_GetDeltaTime();

	if (toggleTimer2 >= 0.0)
		toggleTimer2 -= Clock_GetDeltaTime();
}

void _colorSwitch()
{
	switch (spriteColorCount)
	{
	case PLAY:
		spriteColor = DARKMAGENTA;
		break;
	case PLAY2P:
		spriteColor = DARKCYAN;
		break;
	case LEVEL:
		spriteColor = DARKGREEN;
		break;
	case HISCORE:
		spriteColor = GRAY;
		break;
	case CREDIT:
		spriteColor = DARKYELLOW;
		break;
	case QUIT:
		spriteColor = DARKBLUE;
		break;
	case LEVEL_TUTORIAL:
		spriteColor = DARKMAGENTA;
		break;
	}
}

void _renderChoice()
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
	case CREDITSCREEN:
		_renderCredit();
		break;
	case LEVEL_TUTORIAL:
		Text_Move(&Button_TUTORIAL, 61, 75);
		Text_Render(&Button_TUTORIAL, 0, 0);
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

void _renderTitle()
{
	if (titleTimer <= 0)
	{
		titleShake = (titleShake == 4) ? -4 : 4;
		Text_Render(&title, titleShake, 0);
		titleTimer = 250.0;
	}
	else
	{
		Text_Render(&title, 0, 0);
		titleFlag = 0;
	}
}

void _renderBackground(int offset, CONSOLECOLOR color)
{
	for (int i = 0; i < GAME_WIDTH - offset; i += 4)
		for (int j = 0; j < GAME_HEIGHT - offset; j += 4)
			Console_SetRenderBuffer_CharColor(i + offset, j + offset, '.', color);
}

void _renderCredit()
{
	Text_Render(&Role_GAMEPLAY, 0, 0);
	Text_Render(&Role_AUDIO, 0, 0);
	Text_Render(&Role_LEVEL, 0, 0);
	Text_Render(&Name_FIKRUL, 0, 0);
	Text_Render(&Name_NICO, 0, 0);
	Text_Render(&Name_TED, 0, 0);
	Text_Render(&Name_GUANHIN, 0, 0);
}