#pragma once

#include "Functions.h"
#include "Player/Player.h"
#include "Map/Map.h"
#include "Audio/AudioEngine.h"


void Game_EnterState();
void Game_ExitState();
void Game_ProcessInput();
void Game_Update();
void Game_Render();