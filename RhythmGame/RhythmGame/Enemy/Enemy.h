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

//Initialise Enemy values
extern void Enemy_Init(GAMETYPE type);\

//Updates Enemy AI
extern void Enemy_Update();

//Renders Enemy Art
extern void Enemy_Render();

//Flexibility to include enemy in other states
extern Enemy *Enemy_GetEnemy();

//Controls output when enemy is damaged
extern void Enemy_Damage(int which);