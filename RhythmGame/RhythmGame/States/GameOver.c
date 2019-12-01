#include "GameOver.h"
#include <Windows.h>
#include "StateMachine.h"
#include "../Console/Console.h"
#include "../Text/TextReader.h"
#include <stdbool.h>
#include <stdio.h>
#include "../Text/Font.h"


//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************

// For selection
static bool keyUp, keyDown, keyLeft, keyRight, keyEnter;
static int positionChoice;

// For display
#define MAX_LETTERS 3
#define LETTER_X 50
#define LETTER_Y 100


// For writing score to text file
#define TEXT_PATHNAME "..//RhythmGame//$Resources//TestScore.txt"
static char *path;
static FILE *writeScoreFile;
static int gameScore;

typedef struct Letter {
	int choice;
	sprite letterSprite;
} Letter;
Letter name[MAX_LETTERS];


// Functions
void _PositionName();
void _ChooseLetter(Letter *selection, int dir);
void _InputScore(int first, int second, int third, int score);

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
	}
	else if (!GetAsyncKeyState(VK_LEFT)) keyLeft = false;

	// RIGHT
	if (GetAsyncKeyState(VK_RIGHT) && !keyRight)
	{
		keyRight = true;
		if (positionChoice < MAX_LETTERS - 1)
			positionChoice++;
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

//*********************************************************************************
//									UPDATE
//*********************************************************************************
void GameOver_Update()
{

}


//*********************************************************************************
//									RENDER
//*********************************************************************************
void GameOver_Render()
{
	// Render name choice
	for (int i = 0; i < MAX_LETTERS; i++)
		Text_Render(&name[i].letterSprite, 0, 0);
}


//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void GameOver_EnterState()
{
	gameScore = 123456;

	// Initialise gameover
	_PositionName();

	path = TEXT_PATHNAME;
}

void GameOver_ExitState()
{
	
}

void _PositionName()
{
	for (int i = 0; i < MAX_LETTERS; i++)
	{
		name[i].choice = 0;
		name[i].letterSprite = Text_CreateSprite();
		name[i].letterSprite = Font_ConvertToSprite(0);
		Text_Move(&name[i].letterSprite, LETTER_X + (FONT_SPACING * i), LETTER_Y);
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
	Text_Move(&name[positionChoice].letterSprite, LETTER_X + (FONT_SPACING * positionChoice), LETTER_Y);
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
		fprintf(writeScoreFile, "%d %d %d %d\n",first,second,third,score);
		fclose(writeScoreFile);
	}
}
