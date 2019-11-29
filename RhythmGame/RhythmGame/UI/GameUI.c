#include "GameUI.h"
#include "../Map/Map.h"
#include "../States/StateMachine.h"
#include <stdbool.h>
#include "../Text/TextReader.h"
#include "../Global.h"

#define HEART_SPRITE_WIDTH 10
#define HEART_SPRITE_HEIGHT 8

typedef struct Hearts {
	bool visible;
	sprite heartSprite;
} Hearts;

typedef struct HealthUI {
	int count;
	Vector2d origin;
	Hearts hearts[2][5];
} HealthUI;


static int score = 0;
static int combo = 0;

static HealthUI health = { 0 };

void _InitHealth();
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
	score += amt;
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
	score = 0;
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

}