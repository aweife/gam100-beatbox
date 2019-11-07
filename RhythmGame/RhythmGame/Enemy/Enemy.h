#pragma once

#include "../Console/Console.h"
#include "../Random/Random.h"
#include "../Clock/Clock.h"
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

void E_Init();

void E_Update();

void _getClock();

double E_CalculateBPM(int x);

void _spawnProjectile();

void _updateProjectile();

void E_Render();

void _updateEnemy();