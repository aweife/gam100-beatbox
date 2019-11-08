#pragma once

#include "../Console/Console.h"
#include "../Random/Random.h"
#include "../Clock/Clock.h"
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "../Map/Map.h"
#include "../Functions.h"
#include "../Testread.h"

#define NUMBER_OF_PROJECTILE 10

//Projectile Structure
typedef struct //MUSTLEARNTHIS
{
	int x;
	int y;
	int state;
	int pArrayReady;
} Projectile;

void E_Init();

void E_Update();

/*void _getClock();*/

double E_CalculateBPM(int x);

skullenemy *E_GetEnemy();

Projectile *E_GetProjectile();

void _spawnProjectile();

void _updateProjectile();

void E_Render();

void _updateEnemy();