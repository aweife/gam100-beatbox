/*******************************************************************************
* @filename GameOver.c
* @author(s) Fikrul Islami Bin Abdullah
* @DP emails f.abdullah@digipen.edu
* Brief Description:
* This file contains the code to run the game over screen of the game.
* COPYRIGHT © 2019 BY DIGIPEN CORP, USA. ALL RIGHTS RESERVED.
*******************************************************************************/
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
#include "../Audio/AudioEngine.h"
#include "Highscore.h"

//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************

// Family of Sprites
sprite CryingBeatmanState1;
sprite CryingBeatmanState2;
sprite ReaperState1;
sprite ReaperState2;
sprite GameOver;
sprite GetScore;
sprite GetName;
sprite Enter;

// Animation
#define CENTER_OFFSETX 20
#define CENTER_OFFSETY 10
static bool animate = false;
static bool RETURN_DOWN = false;


// For selection
#define CURSOR_SPEED 600.0
static bool keyUp, keyDown, keyLeft, keyRight, keyEnter;
static int positionChoice;
static double cursorTimer;
static bool visible;

// For score display
#define MAX_DIGITS 10
#define SCORE_X 156
#define SCORE_Y 95
#define DIGIT_SPACING 6

static DisplayScore displayMode;

typedef struct Score {
	int gameScore;
	int digitCount;
	sprite digits[MAX_DIGITS];
}Score;
Score score[2];

// For name display
#define MAX_LETTERS 3
#define LETTER_X 140
#define LETTER_Y 114
#define LETTER_SPACING 8

typedef struct Letter {
	int choice;
	sprite letterSprite;
} Letter;
Letter name[MAX_LETTERS];

// For writing score to text file
static char *path;
static FILE *writeScoreFile;

/*********Internal Functions***********/

//Enables animation of Crying Beatman between states
void _RenderCryingBeatmanAnimation();

//Enables animation of Reaper between states
void _RenderReaperAnimation();

//Set score position
void _PositionScore();

//Set position of name
void _PositionName();

//Enables input of letters in name
void _ChooseLetter(Letter *selection, int dir);

//Inputs the score from the game
void _InputScore(int first, int second, int third, int score);

//Enables animation of cursor
void _BlinkingCursor();

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
		if (displayMode == SOLO)
			_InputScore(name[0].choice, name[1].choice, name[2].choice, score[0].gameScore);

		StateMachine_ChangeState(State_MainMenu);
	}
	else if (!GetAsyncKeyState(VK_RETURN)) keyEnter = false;


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
	animate = Audio_GetSpectrum(1);
	
	// Update cursor
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
	Text_Render(&Enter, 0, 0);

	// Render cursor
	if (displayMode == SOLO)
	{
		// Render name:
		Text_Render(&GetName, 0, 0);

		// Render name choice
		for (int i = 0; i < MAX_LETTERS; i++)
			Text_Render(&name[i].letterSprite, 0, 0);

		// Render linking cursor
		if (visible)
			for (int i = 0, x = LETTER_X - 1 + (LETTER_SPACING * positionChoice), y = LETTER_Y + LETTER_SPACING; i <= FONT_SPACING; i++)
				Console_SetRenderBuffer_CharColor(x + i, y, ' ', WHITE);
	}
	else
		for (int i = 0; i < score[1].digitCount; i++)
			Text_RenderColor(&score[1].digits[i], GREEN, 0, 0);


	// Render scores
	for (int i = 0; i < score[0].digitCount; i++)
		Text_RenderColor(&score[0].digits[i], BLUE, 0, 0);
}


//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void GameOver_EnterState()
{
	CryingBeatmanState1 = Text_CreateSprite();
	Text_Init(&CryingBeatmanState1, "$Resources//Sprites//GameOver//CryingBeatman1.txt");
	Text_Move(&CryingBeatmanState1, (GAME_WIDTH / 4) + CENTER_OFFSETX - 5, (GAME_HEIGHT / 2) - CENTER_OFFSETY - 5);

	CryingBeatmanState2 = Text_CreateSprite();
	Text_Init(&CryingBeatmanState2, "$Resources//Sprites//GameOver//CryingBeatman2.txt");
	Text_Move(&CryingBeatmanState2, (GAME_WIDTH / 4) + CENTER_OFFSETX - 5, (GAME_HEIGHT / 2) - CENTER_OFFSETY - 5);

	ReaperState1 = Text_CreateSprite();
	Text_Init(&ReaperState1, "$Resources//Sprites//GameOver//Reaper1.txt");
	Text_Move(&ReaperState1, (GAME_WIDTH / 2) - CENTER_OFFSETX - 2, (GAME_HEIGHT / 4) - CENTER_OFFSETY - 5);

	ReaperState2 = Text_CreateSprite();
	Text_Init(&ReaperState2, "$Resources//Sprites//GameOver//Reaper2.txt");
	Text_Move(&ReaperState2, (GAME_WIDTH / 2) - CENTER_OFFSETX - 27, (GAME_HEIGHT / 4) - CENTER_OFFSETY - 20);

	GameOver = Text_CreateSprite();
	Text_Init(&GameOver, "$Resources//Sprites//GameOver//GameOver.txt");
	Text_Move(&GameOver, (GAME_WIDTH / 2) - CENTER_OFFSETX - 55, (GAME_HEIGHT / 4) + CENTER_OFFSETY - 5);

	GetScore = Text_CreateSprite();
	Text_Init(&GetScore, "$Resources//Sprites//GameOver//GetScore.txt");
	Text_Move(&GetScore, (GAME_WIDTH / 2) - CENTER_OFFSETX - 75, (GAME_HEIGHT / 4) + CENTER_OFFSETY + 42);

	GetName = Text_CreateSprite();
	Text_Init(&GetName, "$Resources//Sprites//GameOver//GetName.txt");
	Text_Move(&GetName, (GAME_WIDTH / 2) - CENTER_OFFSETX - 59, (GAME_HEIGHT / 4) + CENTER_OFFSETY + 62);

	Enter = Text_CreateSprite();
	Text_Init(&Enter, "$Resources//Sprites//GameOver//GameOver_Enter.txt");
	Text_Move(&Enter, (GAME_WIDTH / 2) - CENTER_OFFSETX - 35, (GAME_HEIGHT / 4) + CENTER_OFFSETY + 85);

	// Init score and name
	_PositionScore();
	_PositionName();

	path = HIGHSCORE_PATH;
}

void GameOver_ExitState()
{
	// Reset score
	score[1].gameScore = score[0].gameScore = 0;

	// Cleanup sprites
	Text_Cleanup(&CryingBeatmanState1);
	Text_Cleanup(&CryingBeatmanState2);
	Text_Cleanup(&ReaperState1);
	Text_Cleanup(&ReaperState2);
	Text_Cleanup(&GameOver);
	Text_Cleanup(&GetScore);
	Text_Cleanup(&GetName);
	Text_Cleanup(&Enter);
}

void GameOver_SetScore(DisplayScore mode, int score1, int score2)
{
	displayMode = mode;
	score[0].gameScore = score1;

	// If its two player
	if (displayMode == NOTSOLO)
		score[1].gameScore = score2;
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

	if (animate == true)
	{
		Text_Render(&ReaperState2, 0, 0);
	}

}

void _PositionScore()
{
	score[0].digitCount = 0;
	for (int i = 0, digit = score[0].gameScore; digit > 0; i++, digit /= 10)
	{
		score[0].digits[i] = Text_CreateSprite();
		score[0].digits[i] = Font_ConvertToSprite(26 + digit % 10);
		Text_Move(&score[0].digits[i], SCORE_X - (DIGIT_SPACING * i), SCORE_Y);
		score[0].digitCount++;
	}

	if (displayMode == NOTSOLO)
	{
		score[1].digitCount = 0;
		for (int i = 0, digit = score[1].gameScore; digit > 0; i++, digit /= 10)
		{
			score[1].digits[i] = Text_CreateSprite();
			score[1].digits[i] = Font_ConvertToSprite(26 + digit % 10);
			Text_Move(&score[1].digits[i], SCORE_X - (DIGIT_SPACING * i), SCORE_Y+DIGIT_SPACING);
			score[1].digitCount++;
		}
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