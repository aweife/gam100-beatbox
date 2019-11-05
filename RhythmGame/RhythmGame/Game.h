#pragma once

#include <Windows.h>
#include "Player/Player.h"
#include "Clock/Clock.h"

void Game_EnterState();
void Game_ExitState();
void Game_ProcessInput();
void Game_Update();
void Game_Render();

void playerMove();
void diagonalCheck();
void playerShape();

void gameRender();