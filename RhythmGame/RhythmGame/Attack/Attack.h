#pragma once

#include "../Global.h"
#include "../Text/TextReader.h"
#include <stdbool.h>

#define NUMBER_OF_PROJECTILE 100
#define LENGTH_OF_LASER 200
#define NUMBER_OF_LASER 4

typedef enum ATTACKTYPE {
	PROJECTILE,
	LASER,
}ATTACKTYPE;

//Projectile Structure
typedef struct Projectile {
	Vector2d position;
	bool active;
	DIRECTION direction;
	sprite projectileSprite;
} Projectile;

typedef struct Laser {
	Vector2d laserPosition;
	DIRECTION direction;
	bool active;
	sprite spawnSprite;
	sprite laserSprite[LENGTH_OF_LASER];
	int laserIndex;
	double tick;
} Laser;

extern void Attack_Init();
extern void Attack_Update();
extern void Attack_Render();
extern void Attack_Spawn(ATTACKTYPE type, Vector2d spawnPosition, DIRECTION direction);
extern void Attack_Cleanup();
