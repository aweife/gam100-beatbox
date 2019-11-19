#pragma once

#include "../Global.h"
#include "../Text/TextReader.h"
#include <stdbool.h>

#define NUMBER_OF_PROJECTILE 50

typedef struct Enemy {
	Vector2d position;
	Vector2d newPosition;
	sprite enemySprite;
} Enemy;

////Projectile Structure
//typedef struct Projectile//MUSTLEARNTHIS
//{
//	Vector2d position;
//	bool visible;
//	bool available;
//} Projectile;

extern void Enemy_Init();
extern void Enemy_FixedUpdate();
extern void Enemy_Render();
extern sprite *Enemy_GetEnemy();
extern void Enemy_MoveTo(int seed, int speed);