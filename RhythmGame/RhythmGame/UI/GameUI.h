#pragma once

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

extern void GameUI_Init();

extern void GameUI_Update();

extern void GameUI_Render();
// Call to decrease hearts
void GameUI_DecreaseHealth(int damage);
