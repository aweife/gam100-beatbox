#pragma once

#include "../Global.h"
#include "../Text/TextReader.h"
#include <stdbool.h>

typedef enum ENEMYSTATE {
	NORMAL,
	DAMAGED,
}ENEMYSTATE;

typedef enum SCORESTATE {
	SMALL,
	MEDIUM,
	BIG,
}SCORESTATE;

typedef struct Enemy {
	Vector2d position;
	Vector2d nextPosition;
	sprite enemySprite;
	double velocity;
	ENEMYSTATE state;
	SCORESTATE scoreState;
} Enemy;


extern void Enemy_Init();
extern void Enemy_Update();
extern void Enemy_Render();
extern sprite *Enemy_GetEnemySprite();
extern void Enemy_Damage();