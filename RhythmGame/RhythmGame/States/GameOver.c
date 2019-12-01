#include "GameOver.h"
#include <Windows.h>
#include "StateMachine.h"
#include "../Console/Console.h"
#include "../Text/TextReader.h"
#include <stdbool.h>
#include <stdio.h>


//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************

// Word sprites
typedef struct Words {
	sprite atoj[10];
	sprite ktot[10];
	sprite utoz[6];
	sprite numbers[10];
} Words;
Words words;

// For selection
static bool keyUp, keyDown, keyLeft, keyRight, keyEnter;
static int positionChoice;

// For display
#define MAX_LETTERS 3
#define LETTER_X 50
#define LETTER_Y 100
#define LETTER_SPACING 6

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
void _InitLetters();
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
void GameOver_EnterState(int score)
{
	gameScore = 123456;
	// Initialise gameover
	_InitLetters();
	_PositionName();

	path = TEXT_PATHNAME;
}

void GameOver_ExitState()
{
	// Destroy a to j
	for (int i = 0; i < 10; i++)
		Text_Cleanup(&words.atoj[i]);

	// Destroy k to t
	for (int i = 0; i < 10; i++)
		Text_Cleanup(&words.ktot[i]);

	// Destroy u to z
	for (int i = 0; i < 6; i++)
		Text_Cleanup(&words.utoz[i]);

	// Destroy numbers
	for (int i = 0; i < 10; i++)
		Text_Cleanup(&words.numbers[i]);
}

void _InitLetters()
{
	// Create a to j
	for (int i = 0; i < 10; i++)
	{
		words.atoj[i] = Text_CreateSprite();
		Text_InitArray(&words.atoj[i], "..//RhythmGame//$Resources//Alphabets.txt", i);
	}
	// Create k to t
	for (int i = 0; i < 10; i++)
	{
		words.ktot[i] = Text_CreateSprite();
		Text_InitArray(&words.ktot[i], "..//RhythmGame//$Resources//Alphabets2.txt", i);
	}
	// Create u to z
	for (int i = 0; i < 6; i++)
	{
		words.utoz[i] = Text_CreateSprite();
		Text_InitArray(&words.utoz[i], "..//RhythmGame//$Resources//Alphabets3.txt", i);
	}
	// Create numbers
	for (int i = 0; i < 10; i++)
	{
		words.numbers[i] = Text_CreateSprite();
		Text_InitArray(&words.numbers[i], "..//RhythmGame//$Resources//numbers.txt", i);
	}
}

void _PositionName()
{
	for (int i = 0; i < MAX_LETTERS; i++)
	{
		name[i].choice = 0;
		name[i].letterSprite = Text_CreateSprite();
		name[i].letterSprite = words.atoj[0];
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

	name[positionChoice].letterSprite = words.atoj[selection->choice];
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
		fprintf(writeScoreFile, "%d %d %d %d\n",first,second,third,score);
		fclose(writeScoreFile);
	}
}
