#pragma once

#include "../Text/TextReader.h"

#define NUMBER_OF_PROJECTILE 10

//Projectile Structure
typedef struct Projectile//MUSTLEARNTHIS
{
	int x;
	int y;
	int state;
	int pArrayReady;
} Projectile;

void Enemy_Init();

void Enemy_Update();

/*void _getClock();*/

double E_CalculateBPM(int x);

sprite *E_GetEnemy();

Projectile *E_GetProjectile();

void _spawnProjectile();

void _updateProjectile();

void Enemy_Render();

void _updateEnemy();