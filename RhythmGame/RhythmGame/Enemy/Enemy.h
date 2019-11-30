#pragma once

#include "../Global.h"
#include "../Text/TextReader.h"
#include <stdbool.h>
#include "../States/Game.h"

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
	Vector2d startPosition;
	Vector2d endPosition;
	Vector2d nextPosition;
	sprite enemySprite;
	double velocity;
	ENEMYSTATE state;
	SCORESTATE scoreState;
} Enemy;


extern void Enemy_Init(GAMETYPE type);
extern void Enemy_Update();
extern void Enemy_Render();
extern Enemy *Enemy_GetEnemy();
extern void Enemy_Damage(int which);