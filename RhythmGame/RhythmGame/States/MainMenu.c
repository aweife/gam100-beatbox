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
sprite Button_ENTER;
sprite Button_PLAY;
sprite Button_2P;
sprite Button_LEVEL;
sprite Button_HISCORE;
sprite Button_CREDIT;
sprite Button_QUIT;
sprite Button_TUTORIAL;
sprite Button_LEVEL1;
sprite leftArrow, rightArrow, upArrow, downArrow;
sprite title;

// CREDIT1 SPRITES
sprite Role_GAMEPLAY;
sprite Role_AUDIO;
sprite Role_LEVEL;
sprite Name_FIKRUL;
sprite Name_NICO;
sprite Name_TED;
sprite Name_GUANHIN;

// CREDIT2 SPRITES
sprite credit2_SCHOOL;
sprite credit2_TEAM;
sprite credit2_PRESENT;
sprite credit2_WEBSITE;
sprite credit2_COPYRIGHT;

// CREDIT3 SPRITES
sprite credit3;

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

static int choice = PRESSENTER;
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

	Button_ENTER = Text_CreateSprite();
	Button_PLAY = Text_CreateSprite();
	Button_2P = Text_CreateSprite();
	Button_LEVEL = Text_CreateSprite();
	Button_HISCORE = Text_CreateSprite();
	Button_CREDIT = Text_CreateSprite();
	Button_QUIT = Text_CreateSprite();	
	Button_TUTORIAL = Text_CreateSprite();
	Button_LEVEL1 = Text_CreateSprite();
	Text_Init(&Button_ENTER, "..//RhythmGame//$Resources//MainMenu//Button_ENTER.txt");
	Text_Init(&Button_PLAY, "..//RhythmGame//$Resources//MainMenu//Button_PLAY.txt");
	Text_Init(&Button_2P, "..//RhythmGame//$Resources//MainMenu//Button_2P.txt");
	Text_Init(&Button_LEVEL, "..//RhythmGame//$Resources//MainMenu//Button_LEVEL.txt");
	Text_Init(&Button_HISCORE, "..//RhythmGame//$Resources//MainMenu//Button_HISCORE.txt");
	Text_Init(&Button_CREDIT, "..//RhythmGame//$Resources//MainMenu//Button_CREDIT.txt");
	Text_Init(&Button_QUIT, "..//RhythmGame//$Resources//MainMenu//Button_QUIT.txt");
	Text_Init(&Button_TUTORIAL, "..//RhythmGame//$Resources//MainMenu//Button_TUTORIAL.txt");
	Text_Init(&Button_LEVEL1, "..//RhythmGame//$Resources//MainMenu//Button_LEVEL1.txt");

	leftArrow = Text_CreateSprite();
	rightArrow = Text_CreateSprite();
	upArrow = Text_CreateSprite();
	downArrow = Text_CreateSprite();
	Text_Init(&leftArrow, "..//RhythmGame//$Resources//MainMenu//Arrow_LEFT.txt");
	Text_Init(&rightArrow, "..//RhythmGame//$Resources//MainMenu//Arrow_RIGHT.txt");
	Text_Init(&upArrow, "..//RhythmGame//$Resources//MainMenu//Arrow_UP.txt");
	Text_Init(&downArrow, "..//RhythmGame//$Resources//MainMenu//Arrow_DOWN.txt");

	title = Text_CreateSprite();
	Text_Init(&title, "..//RhythmGame//$Resources//MainMenu//Title1.txt");

	// CREDIT SPRITES INIT
	Role_GAMEPLAY = Text_CreateSprite();
	Role_AUDIO = Text_CreateSprite();
	Role_LEVEL = Text_CreateSprite();
	Name_FIKRUL = Text_CreateSprite();
	Name_NICO = Text_CreateSprite();
	Name_TED = Text_CreateSprite();
	Name_GUANHIN = Text_CreateSprite();
	Text_Init(&Role_GAMEPLAY, "..//RhythmGame//$Resources//Credit//Role_GAMEPLAY.txt");
	Text_Init(&Role_AUDIO, "..//RhythmGame//$Resources//Credit//Role_AUDIO.txt");
	Text_Init(&Role_LEVEL, "..//RhythmGame//$Resources//Credit//Role_LEVEL.txt");
	Text_Init(&Name_FIKRUL, "..//RhythmGame//$Resources//Credit//Name_FIKRUL.txt");
	Text_Init(&Name_NICO, "..//RhythmGame//$Resources//Credit//Name_NICO.txt");
	Text_Init(&Name_TED, "..//RhythmGame//$Resources//Credit//Name_TED.txt");
	Text_Init(&Name_GUANHIN, "..//RhythmGame//$Resources//Credit//Name_GUANHIN.txt");

	// CREDIT2 SPRITES INIT
	credit2_SCHOOL = Text_CreateSprite();
	Text_Init(&credit2_SCHOOL, "..//RhythmGame//$Resources//Credit//Credit2_SCHOOL.txt");
	credit2_PRESENT = Text_CreateSprite();
	Text_Init(&credit2_PRESENT, "..//RhythmGame//$Resources//Credit//Credit2_PRESENT.txt");
	credit2_TEAM = Text_CreateSprite();
	Text_Init(&credit2_TEAM, "..//RhythmGame//$Resources//Credit//Credit2_TEAM.txt");
	credit2_WEBSITE = Text_CreateSprite();
	Text_Init(&credit2_WEBSITE, "..//RhythmGame//$Resources//Credit//Credit2_WEBSITE.txt");
	credit2_COPYRIGHT = Text_CreateSprite();
	Text_Init(&credit2_COPYRIGHT, "..//RhythmGame//$Resources//Credit//Credit2_COPYRIGHT.txt");

	// CREDIT3 SPRITES INIT
	credit3 = Text_CreateSprite();
	Text_Init(&credit3, "..//RhythmGame//$Resources//Credit//Credit3.txt");

	// CREDIT1 SPRITES UPDATE
	Text_Move(&Role_GAMEPLAY, 50, 40);
	Text_Move(&Role_AUDIO, 70, 80);
	Text_Move(&Role_LEVEL, 35, 110);
	Text_Move(&Name_FIKRUL, 80, 55);
	Text_Move(&Name_NICO, 85, 65);
	Text_Move(&Name_TED, 87, 95);
	Text_Move(&Name_GUANHIN, 75, 125);

	// CREDIT2 SPRITES UPDATE
	Text_Move(&credit2_PRESENT, 40, 27);
	Text_Move(&credit2_SCHOOL, 15, 20);
	Text_Move(&credit2_TEAM, 30, 35);
	Text_Move(&credit2_WEBSITE, 15, 100);
	Text_Move(&credit2_COPYRIGHT, 15, 110);

	// CREDIT3 SPRITES UPDATE
	Text_Move(&credit3, 17, 50);

	Text_Move(&leftArrow, 12, 75);
	Text_Move(&rightArrow, 170, 75);
	Text_Move(&upArrow, 83, 40);
	Text_Move(&downArrow, 83, 125);
	Text_Move(&title, 30, 10);

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
	if (GetAsyncKeyState(VK_RETURN) && keyDown == 0)
		_confirmChoice(choice);

	if (GetAsyncKeyState(VK_LEFT) && keyDown == 0)
	{
		if (choice < LEVEL_TUTORIAL)
			choice--;
		arrowTimer = 100.0;
		shakeDirection = LEFT;
	}

	if (GetAsyncKeyState(VK_RIGHT) && keyDown == 0)
	{
		if (choice < LEVEL_TUTORIAL)
			choice++;
		arrowTimer = 100.0;
		shakeDirection = RIGHT;
	}

	if (GetAsyncKeyState(VK_UP) && keyDown == 0)
	{
		if (choice >= LEVEL_TUTORIAL)
			choice--;
		arrowTimer = 100.0;
		shakeDirection = UP;
	}

	if (GetAsyncKeyState(VK_DOWN) && keyDown == 0)
	{
		if (choice >= LEVEL_TUTORIAL)
			choice++;
		arrowTimer = 100.0;
		shakeDirection = DOWN;
	}

	if (GetAsyncKeyState(VK_RIGHT) || 
		GetAsyncKeyState(VK_LEFT) || 
		GetAsyncKeyState(VK_RETURN) || 
		GetAsyncKeyState(VK_UP) || 
		GetAsyncKeyState(VK_DOWN))
		keyDown = 1;
	else
		keyDown = 0;

	if (choice == PRESSENTER + 1 || choice == PRESSENTER - 1)
		choice = PRESSENTER;

	if (choice == REPEAT)
		choice = PLAY;
	if (choice == PLAY - 1)
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

	if (choice != PRESSENTER)
		_renderArrow();

	if (choice < CREDITSCREEN1)
		_renderBeat();

	_renderChoice(choice);
	
}

//*********************************************************************************
//									FUNCTIONS
//*********************************************************************************
void _confirmChoice()
{
	switch (choice)
	{
	case PRESSENTER:
		choice = 0;
		break;
	case PLAY:
		StateMachine_ChangeMode(ONEPLAYER);
		StateMachine_ChangeState(State_Tutorial);
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
		choice = CREDITSCREEN1;
		break;
	case LEVEL_TUTORIAL:
		StateMachine_ChangeMode(ONEPLAYER);
		StateMachine_ChangeState(State_Tutorial);
		break;
	case LEVEL_LEVEL1:
		StateMachine_ChangeMode(ONEPLAYER);
		StateMachine_ChangeState(State_Game);
		break;
	case QUIT:
		Game_Exit();
		break;
	}
}

void _renderChoice()
{
	if (choice < CREDITSCREEN1)
		_creditExitState();

	switch (choice)
	{
	case PRESSENTER:
		Text_Move(&Button_ENTER, 68, 75);
		Text_Render(&Button_ENTER, 0, 0);
		break;
	case PLAY2P:
		Text_Move(&Button_2P, 68, 75);
		Text_Render(&Button_2P, 0, 0);
		break;
	case LEVEL:
		Text_Move(&Button_LEVEL, 68, 75);
		Text_Render(&Button_LEVEL, 0, 0);
		break;
	case HISCORE:
		Text_Move(&Button_HISCORE, 68, 75);
		Text_Render(&Button_HISCORE, 0, 0);
		break;
	case CREDIT:
		Text_Move(&Button_CREDIT, 68, 75);
		Text_Render(&Button_CREDIT, 0, 0);
		break;
	case QUIT:
		Text_Move(&Button_QUIT, 68, 75);
		Text_Render(&Button_QUIT, 0, 0);
		break;
	case CREDITSCREEN1:
	case CREDITSCREEN2:
	case CREDITSCREEN3:
		_renderCredit();
		break;
	case LEVEL_TUTORIAL:
		Text_Move(&Button_TUTORIAL, 68, 75);
		Text_Render(&Button_TUTORIAL, 0, 0);
		break;
	case LEVEL_LEVEL1:
		Text_Move(&Button_LEVEL1, 68, 75);
		Text_Render(&Button_LEVEL1, 0, 0);
		break;
	default:
		Text_Move(&Button_PLAY, 68, 75);
		Text_Render(&Button_PLAY, 0, 0);
		choice = 0;
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
		Text_Move(&diamond2, 30, 49);
		Text_Render(&diamond2, 0, 0);
		break;
	case 3:
		Text_Move(&diamond3, 28, 48);
		Text_Render(&diamond3, 0, 0);
		break;
	default:
		Text_Move(&diamond1, 32, 50);
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

int ArrowX = 0;
int ArrowY = 0;

void _renderArrow()
{
	if (choice >= LEVEL_TUTORIAL)
	{
		if (choice != CREDITSCREEN1)
			Text_RenderColor(&upArrow, RED, 0, 0);

		Text_RenderColor(&downArrow, RED, 0, 0);
	}
	else
	{
		Text_RenderColor(&leftArrow, RED, 0, 0);
		Text_RenderColor(&rightArrow, RED, 0, 0);
	}
	
	if (arrowTimer < 0)
		shakeDirection = 0;
	else if (arrowTimer >= 0)
	{
		if (shakeDirection == RIGHT && choice < LEVEL_TUTORIAL)
			Text_RenderColor(&rightArrow, RED, 4, 0);
		else if (shakeDirection == LEFT && choice < LEVEL_TUTORIAL)
			Text_RenderColor(&leftArrow, RED, -4, 0);

		if (shakeDirection == UP && choice >= LEVEL_TUTORIAL && choice != CREDITSCREEN1)
			Text_RenderColor(&upArrow, RED, 0, -4);
		else if (shakeDirection == DOWN && choice >= LEVEL_TUTORIAL)
			Text_RenderColor(&downArrow, RED, 0, 4);
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
	switch (choice)
	{
	case CREDITSCREEN1:
		Text_Move(&title, 30, 10);
		Text_Move(&downArrow, 83, 140);
		Text_Render(&Role_GAMEPLAY, 0, 0);
		Text_Render(&Role_AUDIO, 0, 0);
		Text_Render(&Role_LEVEL, 0, 0);
		Text_Render(&Name_FIKRUL, 0, 0);
		Text_Render(&Name_NICO, 0, 0);
		Text_Render(&Name_TED, 0, 0);
		Text_Render(&Name_GUANHIN, 0, 0);
		break;
	case CREDITSCREEN2:
		Text_Move(&title, 32, 60);
		Text_Move(&upArrow, 83, 5);
		Text_Render(&credit2_SCHOOL, 0, 0);
		Text_Render(&credit2_TEAM, 0, 0);
		Text_Render(&credit2_PRESENT, 0, 0);
		Text_Render(&credit2_WEBSITE, 0, 0);
		Text_Render(&credit2_COPYRIGHT, 0, 0);
		break;
	case CREDITSCREEN3:
		Text_Move(&title, 32, 20);
		Text_Render(&credit3, 0, 0);
		break;
	}
}

void _creditExitState()
{
	Text_Move(&upArrow, 83, 40);
	Text_Move(&downArrow, 83, 125);
	Text_Move(&title, 30, 10);
}