#include "GameUI.h"
#include "../Map/Map.h"
#include "../States/StateMachine.h"
#include <stdbool.h>
#include "../Text/TextReader.h"
#include "../Global.h"

#define HEART_SPRITE_WIDTH 10
#define HEART_SPRITE_HEIGHT 8

#define MAX_SCORE_DIGITS 10
#define NUMBER_SPACING 4

typedef struct Hearts {
	bool visible;
	sprite heartSprite;
} Hearts;

typedef struct HealthUI {
	int count;
	Vector2d origin;
	Hearts hearts[2][5];
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
static HealthUI health = { 0 };
static ScoreUI score;

void _InitNumbers();
void _InitHealth();
void _InitScore();
void _AlignScore();
void _RenderHealth();
void _RenderScore();

void GameUI_Init()
{
	_InitNumbers();
	_InitHealth();
	_InitScore();
}

void GameUI_Update()
{

}

void GameUI_Render()
{
	_RenderHealth();
	_RenderScore();
}

void GameUI_DecreaseHealth(int damage)
{
	if (health.count > 0)
		health.count--;
	else
		StateMachine_ChangeState(State_GameOver);
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
	// Set health
	health.count = 10;

	// Calculate origin position on screen
	health.origin.x = MAP_OFFSET;
	health.origin.y = MAP_OFFSET;

	// Init hearts
	for (int i = 0; i < health.count / 5; i++)
	{
		for (int j = 0; j < health.count / 2; j++)
		{
			health.hearts[i][j].visible = true;
			Text_Init(&health.hearts[i][j].heartSprite, "..//RhythmGame//$Resources//Health2.txt");
			Text_Move(&health.hearts[i][j].heartSprite, health.origin.x + (HEART_SPRITE_WIDTH * j), health.origin.y + (HEART_SPRITE_HEIGHT * i));
		}
	}
}

void _InitScore()
{
	// Init score
	score.count = 0;

	// Init position on screen
	score.origin.x = GAME_WIDTH - MAP_OFFSET - NUMBER_SPACING - 1;
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
	// Then first row
	for (int i = 0; i < health.count; i++)
		if (health.hearts[0][i].visible)
			Text_Render(&health.hearts[0][i].heartSprite, 0, 0);
}

void _RenderScore()
{
	for (int i = 0; i < MAX_SCORE_DIGITS; i++)
		if (score.digits[i].visible)
			Text_RenderColor(&score.digits[i].numberSprite, score.digits[i].color, 0, 0);
}