#pragma once

#include "../Global.h"
#include "../Text/TextReader.h"
#include <stdbool.h>


typedef struct Enemy {
	Vector2d position;
	Vector2d nextPosition;
	sprite enemySprite;
	double velocity;
} Enemy;


extern void Enemy_Init();
extern void Enemy_Update();
extern void Enemy_Render();
extern Enemy *Enemy_GetEnemy();