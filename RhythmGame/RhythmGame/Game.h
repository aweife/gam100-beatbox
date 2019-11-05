#pragma once

#include <Windows.h>
#include "Player/Player.h"
#include "Clock/Clock.h"
#include "Console/Console.h"

void Game_EnterState();
void Game_ExitState();
void Game_ProcessInput();
void Game_Update();
void Game_Render();