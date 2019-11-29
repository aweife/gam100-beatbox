#include "GameUI.h"
#include "../Map/Map.h"
#include "../States/StateMachine.h"
#include <stdbool.h>
#include "../Text/TextReader.h"
#include "../Global.h"

#define HEART_SPRITE_WIDTH 10
#define HEART_SPRITE_HEIGHT 8

#define MAX_SCORE_DIGITS 5

typedef struct Hearts {
	bool visible;
	sprite heartSprite;
} Hearts;

typedef struct HealthUI {
	int count;
	Vector2d origin;
	Hearts hearts[2][5];
} HealthUI;

typedef struct ScoreDigit {
	bool visible;
	sprite numbers[10];
} ScoreDigit;

typedef struct ScoreUI {
	Vector2d origin;
	int count;
	ScoreDigit digits[MAX_SCORE_DIGITS];
} ScoreUI;

static HealthUI health = { 0 };
static ScoreUI score;

void _InitHealth();
void _InitScore();
void _RenderHealth();
void _RenderScore();

void GameUI_Init()
{
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
	if (health.count)
		health.count--;
	else
		StateMachine_ChangeState(State_GameOver);
}

void GameUI_AddScore(int amt)
{

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
	// Init sprites for numbers
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < MAX_SCORE_DIGITS; j++)
		{
			score.digits[j].visible = false;
			Text_InitArray(&score.digits[j].numbers[i], "..//RhythmGame//$Resources//numbers.txt", i);
		}

	// Init score
	score.count = 0;

	// Init position on screen
	score.origin.x = GAME_WIDTH - MAP_OFFSET;
	score.origin.y = MAP_OFFSET;

	score.digits[0].visible = true;
	Text_Move(&score.digits[0].numbers[5], score.origin.x, score.origin.y);
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
	//for (int i = 0; i < MAX_SCORE_DIGITS; i++)
		//if (score.digits[i].visible)
			Text_Render(&score.digits[0].numbers[5], score.origin.x, score.origin.y);

}