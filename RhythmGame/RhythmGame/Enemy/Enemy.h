#pragma once

#define _CRT_SECURE_NO_DEPRECATE
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

void E_Init();

void E_Update();

/*void _getClock();*/

double E_CalculateBPM(int x);

void _spawnProjectile();

void _updateProjectile();

void E_Render();

void _updateEnemy();