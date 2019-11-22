#pragma once

#include "../Global.h"
#include "../Text/TextReader.h"
#include <stdbool.h>

#define NUMBER_OF_PROJECTILE 100
#define LENGTH_OF_LASER 200

//Projectile Structure
typedef struct Projectile {
	Vector2d position;
	bool visible;
	DIRECTION direction;
	int speed;
	int distanceToTravel;
	char debugChar;
	sprite projectileSprite;
} Projectile;

//void _chooseAttack();
extern void Attack_Init();
extern void Attack_Update();
extern void Attack_FixedUpdate();
extern void Attack_Render();
extern void Attack_SpawnProjectile(Vector2d spawnPosition, DIRECTION direction, int speed, int distance);
extern void Attack_SpawnLaser(Vector2d spawnPosition, DIRECTION direction, int length);
extern void Attack_Cleanup();
//void Attack_spawnClusterBomb();
//void Attack_updateClusterBomb();

/* extern Projectile* Enemy_GetProjectile(); */
