#include "GameOver.h"
#include <Windows.h>
#include "StateMachine.h"
#include "Game.h"
#include "../Global.h"
#include "../Text/TextReader.h"
#include "../Console/Console.h"
#include "../Text/TextReader.h"
#include <stdbool.h>
#include <stdio.h>
#include "../Text/Font.h"
#include "../Clock/Clock.h"

#define CENTER_OFFSETX 20
#define CENTER_OFFSETY 10

//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************

//Family of Sprites
sprite CryingBeatmanState1;
sprite CryingBeatmanState2;
sprite ReaperState1;
sprite ReaperState2;
sprite GameOver;
sprite GetScore;
sprite GetName;
sprite Enter;
// For selection
#define CURSOR_SPEED 1000.0
static bool keyUp, keyDown, keyLeft, keyRight, keyEnter;
static int positionChoice;
static double cursorTimer;
static bool visible;

// For score display
#define MAX_DIGITS 10
#define SCORE_X 100
#define SCORE_Y 80
#define DIGIT_SPACING 6
static int gameScore;
static int digitCount;

sprite digits[MAX_DIGITS];

// For name display
#define MAX_LETTERS 3
#define LETTER_X 100
#define LETTER_Y 100
#define LETTER_SPACING 8

typedef struct Letter {
	int choice;
	sprite letterSprite;
} Letter;
Letter name[MAX_LETTERS];

// For writing score to text file
#define TEXT_PATHNAME "..//RhythmGame//$Resources//TestScore.txt"
static char *path;
static FILE *writeScoreFile;

// Functions
void _PositionScore();
void _PositionName();
void _ChooseLetter(Letter *selection, int dir);
void _InputScore(int first, int second, int third, int score);
void _BlinkingCursor();

//Static Variables
static double animationDuration = 0.0;
static bool animate = false;
static bool RETURN_DOWN = false;

//*********************************************************************************
//									INPUT
//*********************************************************************************
void GameOver_ProcessInput()
{
	// UP
	if (GetAsyncKeyState(VK_UP) && !keyUp)
	{
		keyUp = true;
		_ChooseLetter(&name[positionChoice], 1);
	}
	else if (!GetAsyncKeyState(VK_UP)) keyUp = false;

	// DOWN
	if (GetAsyncKeyState(VK_DOWN) && !keyDown)
	{
		keyDown = true;
		_ChooseLetter(&name[positionChoice], -1);
	}
	else if (!GetAsyncKeyState(VK_DOWN)) keyDown = false;

	// LEFT
	if (GetAsyncKeyState(VK_LEFT) && !keyLeft)
	{
		keyLeft = true;
		if (positionChoice > 0)
			positionChoice--;

		visible = true;
		cursorTimer = CURSOR_SPEED;
	}
	else if (!GetAsyncKeyState(VK_LEFT)) keyLeft = false;

	// RIGHT
	if (GetAsyncKeyState(VK_RIGHT) && !keyRight)
	{
		keyRight = true;
		if (positionChoice < MAX_LETTERS - 1)
			positionChoice++;

		visible = true;
		cursorTimer = CURSOR_SPEED;
	}
	else if (!GetAsyncKeyState(VK_RIGHT)) keyRight = false;

	// ENTER
	if (GetAsyncKeyState(VK_RETURN) && !keyEnter)
	{
		keyEnter = true;
		_InputScore(name[0].choice, name[1].choice, name[2].choice, gameScore);
	}
	else if (!GetAsyncKeyState(VK_RETURN)) keyEnter = false;
}

	if (GetAsyncKeyState(VK_ESCAPE) && !RETURN_DOWN)
	{
		Game_Exit();
	}
	else if (!GetAsyncKeyState(VK_ESCAPE))
	{
		RETURN_DOWN = false;
	}
}

//*********************************************************************************
//									UPDATE
//*********************************************************************************
void GameOver_Update()
{
	animationDuration += Clock_GetDeltaTime();

	//Logic to Animate in between to Sprite States
	if (animationDuration >= 0.0 && animationDuration <= 500.0)
	{
		animate = false;
	}
	else if (animationDuration >= 500.0 && animationDuration <= 1000.0)
	{
		animate = true;
		animationDuration -= 1000.0;
	}
}
	_BlinkingCursor();
}

//*********************************************************************************
//									RENDER
//*********************************************************************************
void GameOver_Render()
{
	
	Text_Render(&GameOver, 0, 0);
	_RenderCryingBeatmanAnimation();
	_RenderReaperAnimation();
	Text_Render(&GetScore, 0, 0);
	Text_Render(&GetName, 0, 0);
	Text_Render(&Enter, 0, 0);
}

//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void GameOver_EnterState()
{
	CryingBeatmanState1 = Text_CreateSprite();
	Text_Init(&CryingBeatmanState1, "..//RhythmGame//$Resources//CryingBeatman1.txt");
	Text_Move(&CryingBeatmanState1, (GAME_WIDTH / 4) + CENTER_OFFSETX - 5, (GAME_HEIGHT / 2) - CENTER_OFFSETY - 5);

	CryingBeatmanState2 = Text_CreateSprite();
	Text_Init(&CryingBeatmanState2, "..//RhythmGame//$Resources//CryingBeatman2.txt");
	Text_Move(&CryingBeatmanState2, (GAME_WIDTH / 4) + CENTER_OFFSETX - 5, (GAME_HEIGHT / 2) - CENTER_OFFSETY - 5);

	ReaperState1 = Text_CreateSprite();
	Text_Init(&ReaperState1, "..//RhythmGame//$Resources//Reaper1.txt");
	Text_Move(&ReaperState1, (GAME_WIDTH / 2) - CENTER_OFFSETX - 2, (GAME_HEIGHT / 4) - CENTER_OFFSETY - 5);

	ReaperState2 = Text_CreateSprite();
	Text_Init(&ReaperState2, "..//RhythmGame//$Resources//Reaper2.txt");
	Text_Move(&ReaperState2, (GAME_WIDTH / 2) - CENTER_OFFSETX - 27, (GAME_HEIGHT / 4) - CENTER_OFFSETY - 20);

	GameOver = Text_CreateSprite();
	Text_Init(&GameOver, "..//RhythmGame//$Resources//GameOver.txt");
	Text_Move(&GameOver, (GAME_WIDTH / 2) - CENTER_OFFSETX - 55, (GAME_HEIGHT / 4) + CENTER_OFFSETY - 5);

	GetScore = Text_CreateSprite();
	Text_Init(&GetScore, "..//RhythmGame//$Resources//GetScore.txt");
	Text_Move(&GetScore, (GAME_WIDTH / 2) - CENTER_OFFSETX - 75, (GAME_HEIGHT / 4) + CENTER_OFFSETY + 42);
	// Render name choice
	for (int i = 0; i < MAX_LETTERS; i++)
		Text_Render(&name[i].letterSprite, 0, 0);

	// Render nscore
	for (int i = 0; i < digitCount; i++)
		Text_Render(&digits[i], 0, 0);
	GetName = Text_CreateSprite();
	Text_Init(&GetName, "..//RhythmGame//$Resources//GetName.txt");
	Text_Move(&GetName, (GAME_WIDTH / 2) - CENTER_OFFSETX - 59, (GAME_HEIGHT / 4) + CENTER_OFFSETY + 62);

	Enter = Text_CreateSprite();
	Text_Init(&Enter, "..//RhythmGame//$Resources//GameOver_Enter.txt");
	Text_Move(&Enter, (GAME_WIDTH / 2) - CENTER_OFFSETX - 32, (GAME_HEIGHT / 4) + CENTER_OFFSETY + 85);
}
	if (visible)
		for (int i = 0, x = LETTER_X-1 + (LETTER_SPACING * positionChoice), y = LETTER_Y + LETTER_SPACING; i <= FONT_SPACING; i++)
			Console_SetRenderBuffer_CharColor(x + i, y, ' ', WHITE);
}

void GameOver_ExitState()
{
}

//*********************************************************************************
//								INTERNAL FUNCTIONS
//*********************************************************************************

void _RenderCryingBeatmanAnimation()
{
	if (animate == false)
	{
		Text_Render(&CryingBeatmanState1, 0, 0);
	}

	if (animate == true)
	{
		Text_Render(&CryingBeatmanState2, 0, 0);
	}
}

void _RenderReaperAnimation()
{
	if (animate == false)
	{
		Text_Render(&ReaperState1, 0, 0);
	}
void GameOver_EnterState()
{
	// Initialise gameover
	gameScore = 123456;
	_PositionScore();
	_PositionName();

	path = TEXT_PATHNAME;
}

void GameOver_ExitState()
{

}

void _PositionScore()
{
	digitCount = 0;
	for (int i = 0, digit = gameScore; digit > 0; i++, digit /= 10)
	{
		digits[i] = Text_CreateSprite();
		digits[i] = Font_ConvertToSprite(26+digit % 10);
		Text_Move(&digits[i], SCORE_X - (DIGIT_SPACING * i), SCORE_Y);
		digitCount++;
	}
}

void _PositionName()
{
	for (int i = 0; i < MAX_LETTERS; i++)
	{
		name[i].choice = 0;
		name[i].letterSprite = Text_CreateSprite();
		name[i].letterSprite = Font_ConvertToSprite(0);
		Text_Move(&name[i].letterSprite, LETTER_X + (LETTER_SPACING * i), LETTER_Y);
	}
}

void _ChooseLetter(Letter *selection, int dir)
{
	// Increment choice first
	selection->choice += dir;

	// Make sure once letterchoice reaches '9', reset to 'A' and vice versa
	if (selection->choice > 35)
		selection->choice = 0;
	else if (selection->choice < 0)
		selection->choice = 35;

	name[positionChoice].letterSprite = Font_ConvertToSprite(selection->choice);
	Text_Move(&name[positionChoice].letterSprite, LETTER_X + (LETTER_SPACING * positionChoice), LETTER_Y);
}

//Write Score to File
void _InputScore(int first, int second, int third, int score)
{
	fopen_s(&writeScoreFile, path, "a");
	if (writeScoreFile == NULL)
	{
		perror("Error opening file");
	}
	else
	{
		fprintf(writeScoreFile, "%d %d %d %d\n", first, second, third, score);
		fclose(writeScoreFile);
	}
}

void _BlinkingCursor()
{
	if (cursorTimer > 0.0)
		cursorTimer -= Clock_GetDeltaTime();
	else if (cursorTimer <= 0.0)
	{
		cursorTimer = CURSOR_SPEED;
		visible = !visible;
	}
}
