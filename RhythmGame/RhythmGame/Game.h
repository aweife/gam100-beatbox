#pragma once

#include "Functions.h"
#include "Audio/AudioEngine.h"

#include "Player/Player.h"
#include "Map/Map.h"
#include "Enemy/Enemy.h"
#include "GameOver.h"



void Game_EnterState();
void Game_ExitState();
void Game_ProcessInput();
void Game_Update();
void Game_Render();