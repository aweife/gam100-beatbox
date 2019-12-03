#pragma once

#include "../Global.h"
#include "../States/Game.h"

#define GAMEUI_OFFSET 8

// Call to displayer game ui
extern void GameUI_Init(GAMETYPE type);

// Display
extern void GameUI_Render();

// Call to decrease hearts
void GameUI_DecreaseHealth(int damage, int which);

// Call to increase score (contains logic for separating big numbers)
void GameUI_AddScore(int amt, int which);
