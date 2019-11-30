#include "GameUI.h"
#include "../Map/Map.h"
#include "../States/StateMachine.h"
#include <stdbool.h>
#include "../Text/TextReader.h"
#include "../Global.h"


#define HEART_SPRITE_WIDTH 7
#define HEART_SPRITE_HEIGHT 6
#define HEART_ROWS 2
#define HEART_SPRITES 2
#define HEARTS_PER_ROW 5

#define MAX_SCORE_DIGITS 10
#define NUMBER_SPACING 4

typedef struct Hearts {
	bool visible;
	sprite heartSprite;
} Hearts;

typedef struct HealthUI {
	int count;
	Vector2d origin;
	Hearts hearts[5][2];
} HealthUI;

typedef struct Digits {
	bool visible;
	sprite numberSprite;
	CONSOLECOLOR color;
} Digits;

typedef struct ScoreUI {
	Vector2d origin;
	int count;
	Digits digits[MAX_SCORE_DIGITS];
} ScoreUI;

static sprite numbers[10] = { 0 };
static HealthUI health[2] = { 0 };
static ScoreUI score;

// Ui mode
GAMETYPE uiMode;

void _InitNumbers();
void _InitHealth();
void _InitScore();
void _AlignScore();
void _RenderHealth();
void _RenderScore();

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
		if (health[which].count <= 1)
			StateMachine_ChangeState(State_GameOver);
		else
			health[which].count--;
	}
	else
	{
		if (health[which+1].count <= 1)
		{
			if (health[which].count <= 1)
				StateMachine_ChangeState(State_GameOver);
			else health[which].count--;
		}
		else
			health[which+1].count--;
	}
}

void GameUI_AddScore(int amt)
{
	score.count += amt;

	for (int i = 0, digit = score.count; digit > 0; i++, digit /= 10)
	{
		score.digits[i].numberSprite = numbers[digit % 10];
		score.digits[i].color = WHITE;
	}

	// Align again
	_AlignScore();
}

void _InitHealth()
{
	for (int i = 0; i < HEART_ROWS; i++)
	{
		// Set health
		health[i].count = HEART_ROWS * HEARTS_PER_ROW;

		// Set health origin
		health[i].origin.x = GAMEUI_OFFSET;
		health[i].origin.y = MAP_OFFSET + (HEART_SPRITE_HEIGHT * i);

		// Create sprites
		for (int j = 0; j < HEARTS_PER_ROW; j++)
			for (int k = 0; k < HEART_SPRITES; k++)
			{
				health[i].hearts[j][k].visible = true;
				health[i].hearts[j][k].heartSprite = Text_CreateSprite();
				Text_InitArray(&health[i].hearts[j][k].heartSprite, "..//RhythmGame//$Resources//health2.txt", k);
				Text_Move(&health[i].hearts[j][k].heartSprite, health[i].origin.x +
					(HEART_SPRITE_WIDTH * HEART_SPRITES * j) +
					(HEART_SPRITE_WIDTH * k), health[i].origin.y +
					(HEART_SPRITE_HEIGHT * i));
			}
	}
}

void _InitScore()
{
	// Init score
	score.count = 0;

	// Init position on screen
	score.origin.x = GAME_WIDTH - GAMEUI_OFFSET - NUMBER_SPACING - 1;
	score.origin.y = MAP_OFFSET;

	for (int i = 0; i < MAX_SCORE_DIGITS; i++)
	{
		score.digits[i].visible = true;
		score.digits[i].numberSprite = numbers[0];
		score.digits[i].color = GRAY;
	}

	_AlignScore();
}

void _InitNumbers()
{
	// Init sprites for numbers
	for (int i = 0; i < 10; i++)
	{
		numbers[i] = Text_CreateSprite();
		Text_InitArray(&numbers[i], "..//RhythmGame//$Resources//numbers.txt", i);
	}
}

void _AlignScore()
{
	for (int i = 0; i < MAX_SCORE_DIGITS; i++)
		Text_Move(&score.digits[i].numberSprite, score.origin.x - (NUMBER_SPACING * i), score.origin.y);
}

void _RenderHealth()
{
	if (uiMode == ONEPLAYER)
	{
		for (int i = 0; i < HEART_ROWS; i++)
			for (int j = 0; j < health[i].count; j++)
				if (health[i].hearts[0][j].visible)
					Text_Render(&health[i].hearts[0][j].heartSprite, 0, 0);
	}
	else
	{
		CONSOLECOLOR red, darkred;
	}
}

void _RenderScore()
{
	for (int i = 0; i < MAX_SCORE_DIGITS; i++)
		if (score.digits[i].visible)
			Text_RenderColor(&score.digits[i].numberSprite, score.digits[i].color, 0, 0);
}