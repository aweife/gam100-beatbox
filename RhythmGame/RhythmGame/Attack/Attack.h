#pragma once

#include "../Global.h"
#include "../Text/TextReader.h"
#include <stdbool.h>
#include "../Player/Player.h"

#define NUMBER_OF_PROJECTILE 100

//Projectile Structure
typedef struct Projectile
{
	Vector2d position;
	bool visible;
	bool available;
	DIRECTION direction;
	int speed;
	int distanceToTravel;
	sprite projectileSprite;
} Projectile;

// To spawn a projectile
// Attack_Spawn(myown.position);

//ClusterBomb Structure
typedef struct ClusterBomb
{
	Vector2d position;
	bool visible;
	bool available;
	int speed;
	int distance;
} ClusterBomb;

//void _chooseAttack();
extern void Attack_Init();
extern void Attack_Update();
extern void Attack_Render();
extern void Attack_SpawnProjectile(Vector2d spawnPosition, DIRECTION direction, int speed);
//void Attack_spawnLaser();
//void Attack_updateLaser();
//void Attack_spawnClusterBomb();
//void Attack_updateClusterBomb();

/* extern Projectile* Enemy_GetProjectile(); */
