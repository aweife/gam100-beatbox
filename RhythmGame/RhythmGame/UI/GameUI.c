/*******************************************************************************
* @filename Game UI
* @author(s) Ang Wei Feng (Ted)
* @DP emails weifeng.ang@digipen.edu
* Brief Description:
* This file displays the UI for the game (depending on number of players).
* COPYRIGHT © 2019 BY DIGIPEN CORP, USA. ALL RIGHTS RESERVED.
*******************************************************************************/

#include "GameUI.h"
#include "../Map/Map.h"
#include "../States/StateMachine.h"
#include <stdbool.h>
#include "../Text/TextReader.h"
#include "../Global.h"
#include "../States/GameOver.h"

// Hearts
#define HEART_SPRITE_WIDTH 5
#define HEART_SPRITE_HEIGHT 5
#define HEART_SPACING 11
#define HEART_ROWS 2
#define HEART_SPRITES 2
#define HEARTS_PER_ROW 5

// Score
#define MAX_SCORE_DIGITS 10
#define NUMBER_SPACING 6
#define SCORE_ROWS 2 
#define SCORE_HEIGHT 10

// Heart sprite
typedef struct Hearts {
	bool visible;
	sprite heartSprite;
} Hearts;

// The health UI container
typedef struct HealthUI {
	int count;
	Vector2d origin;
	Hearts hearts[5][2];
} HealthUI;

// Digit sprite
typedef struct Digits {
	bool visible;
	sprite numberSprite;
	CONSOLECOLOR color;
} Digits;

// The score container
typedef struct ScoreUI {
	Vector2d origin;
	int count;
	Digits digits[MAX_SCORE_DIGITS];
} ScoreUI;

// For display the numbers logic
static sprite numbers[10] = { 0 };

// UI
static HealthUI health[2] = { 0 };
static ScoreUI score[2] = { 0 };

// UI mode
GAMETYPE uiMode;

/* Private functions **********************************************************/

// Create the numbers sprites to use in the score
void _InitNumbers();

// Create and position the health ui
void _InitHealth();

// Create and position the score ui
void _InitScore();

// To reset the score ui into position everytime it updates
void _AlignScore(ScoreUI *score);

// Render health UI
void _RenderHealth();

// Render score UI
void _RenderScore();

/* Definitions ****************************************************************/

void GameUI_Init(GAMETYPE type)
{
	uiMode = type;
	_InitNumbers();
	_InitHealth();
	_InitScore();
}

void GameUI_Render()
{
	_RenderHealth();
	_RenderScore();
}

void GameUI_DecreaseHealth(int damage, int which)
{
	if (uiMode == TWOPLAYER)
	{
		if (health[!which].count <= 1)
			StateMachine_ChangeState(State_GameOver);
		else
			health[!which].count -= damage;
	}
	else
	{
		if (health[which + 1].count <= 1)
		{
			if (health[which].count < 1)
				StateMachine_ChangeState(State_GameOver);
			else health[which].count -= damage;
		}
		else
			health[which + 1].count -= damage;
	}
}

void GameUI_AddScore(int amt, int which)
{
	score[which].count += amt;

	for (int i = 0, digit = score[which].count; digit > 0; i++, digit /= 10)
	{
		score[which].digits[i].numberSprite = numbers[digit % 10];
		score[which].digits[i].color = uiMode == ONEPLAYER ? WHITE : which ? GREEN : BLUE;
	}

	if (uiMode == ONEPLAYER)
		GameOver_SetScore(SOLO, score[0].count, 0);
	else
		GameOver_SetScore(NOTSOLO, score[0].count, score[1].count);

	// Align again
	_AlignScore(&score[which]);
}

void _InitHealth()
{
	for (int i = 0; i < HEART_ROWS; i++)
	{
		// Set health
		health[i].count = HEART_ROWS * HEARTS_PER_ROW;

		// Set health origin
		health[i].origin.x = GAMEUI_OFFSET;
		health[i].origin.y = GAMEUI_OFFSET - MAP_OFFSET + (HEART_SPRITE_HEIGHT * i);

		// Create sprites
		for (int j = 0; j < HEARTS_PER_ROW; j++)
			for (int k = 0; k < HEART_SPRITES; k++)
			{
				health[i].hearts[j][k].visible = true;
				health[i].hearts[j][k].heartSprite = Text_CreateSprite();
				Text_InitArray(&health[i].hearts[j][k].heartSprite, "$Resources//Sprites//UI//Heart.txt", k);
				Text_Move(&health[i].hearts[j][k].heartSprite, health[i].origin.x +
					(HEART_SPACING * j) +
					(HEART_SPRITE_WIDTH * k), health[i].origin.y +
					(HEART_SPRITE_HEIGHT * i));
			}

		// Change color if two players
		// Player one color
		if (uiMode == TWOPLAYER)
		{
			CONSOLECOLOR red = GREEN, darkred = DARKGREEN;
			for (int i = 0; i < HEARTS_PER_ROW; i++)
				for (int j = 0; j < HEART_SPRITES; j++)
					for (int k = 0, count = health[0].hearts[i][j].heartSprite.charCount; k < count; k++)
						if (health[0].hearts[i][j].heartSprite.spriteI[k].printchar == 'r')
							health[0].hearts[i][j].heartSprite.spriteI[k].printColor = red;
						else if (health[0].hearts[i][j].heartSprite.spriteI[k].printchar == 'R')
							health[0].hearts[i][j].heartSprite.spriteI[k].printColor = darkred;

			// Player two color
			red = BLUE; darkred = DARKBLUE;
			for (int i = 0; i < HEARTS_PER_ROW; i++)
				for (int j = 0; j < HEART_SPRITES; j++)
					for (int k = 0, count = health[1].hearts[i][j].heartSprite.charCount; k < count; k++)
						if (health[1].hearts[i][j].heartSprite.spriteI[k].printchar == 'r')
							health[1].hearts[i][j].heartSprite.spriteI[k].printColor = red;
						else if (health[1].hearts[i][j].heartSprite.spriteI[k].printchar == 'R')
							health[1].hearts[i][j].heartSprite.spriteI[k].printColor = darkred;
		}
	}
}

void _InitScore()
{
	// Init score
	score[0].count = 0;

	// Init position on screen
	score[0].origin.x = GAME_WIDTH - GAMEUI_OFFSET - NUMBER_SPACING - 1;
	score[0].origin.y = MAP_OFFSET + SCORE_HEIGHT;

	for (int i = 0; i < MAX_SCORE_DIGITS; i++)
	{
		score[0].digits[i].visible = true;
		score[0].digits[i].numberSprite = numbers[0];
		score[0].digits[i].color = GRAY;
	}

	_AlignScore(&score[0]);

	if (uiMode == TWOPLAYER)
	{
		// Init score
		score[1].count = 0;

		// Init position on screen
		score[1].origin.x = GAME_WIDTH - GAMEUI_OFFSET - NUMBER_SPACING - 1;
		score[1].origin.y = MAP_OFFSET;

		for (int i = 0; i < MAX_SCORE_DIGITS; i++)
		{
			score[1].digits[i].visible = true;
			score[1].digits[i].numberSprite = numbers[0];
			score[1].digits[i].color = GRAY;
		}

		_AlignScore(&score[1]);
	}
}

void _InitNumbers()
{
	// Init sprites for numbers
	for (int i = 0; i < 10; i++)
	{
		numbers[i] = Text_CreateSprite();
		Text_InitArray(&numbers[i], "$Resources//Sprites//UI//numbers.txt", i);
	}
}

void _AlignScore(ScoreUI *score)
{
	for (int i = 0; i < MAX_SCORE_DIGITS; i++)
		Text_Move(&score->digits[i].numberSprite, score->origin.x - (NUMBER_SPACING * i), score->origin.y);
}

void _RenderHealth()
{
	for (int i = 0; i < HEART_ROWS; i++)
		for (int j = 0; j < health[i].count; j++)
			if (health[i].hearts[0][j].visible)
				Text_Render(&health[i].hearts[0][j].heartSprite, 0, 0);
}

void _RenderScore()
{
	for (int i = 0; i < MAX_SCORE_DIGITS; i++)
		if (score[0].digits[i].visible)
			Text_RenderColor(&score[0].digits[i].numberSprite, score[0].digits[i].color, 0, 0);

	if (uiMode == TWOPLAYER)
	{
		for (int i = 0; i < MAX_SCORE_DIGITS; i++)
			if (score[1].digits[i].visible)
				Text_RenderColor(&score[1].digits[i].numberSprite, score[1].digits[i].color, 0, 0);
	}
}