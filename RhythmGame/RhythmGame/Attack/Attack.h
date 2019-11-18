#pragma once

#include "../Global.h"
#include "../Text/TextReader.h"
#include <stdbool.h>
#include "../Player/Player.h"

#define NUMBER_OF_PROJECTILE 100
#define LENGTH_OF_LASER 100

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
typedef struct Laser
{
	Vector2d position;
	bool visible;
	bool available;
	DIRECTION direction;
	int distanceToTravel;
	sprite laserSprite;
} Laser;

//void _chooseAttack();
extern void Attack_Init();
extern void Attack_FixedUpdate();
extern void Attack_Render();
extern void Attack_SpawnProjectile(Vector2d spawnPosition, DIRECTION direction, int speed, int distance);
extern void Attack_SpawnLaser(Vector2d spawnPosition, DIRECTION direction, int distance);
void Attack_UpdateLaser();
void Attack_UpdateProjectile();
//void Attack_spawnClusterBomb();
//void Attack_updateClusterBomb();

/* extern Projectile* Enemy_GetProjectile(); */
