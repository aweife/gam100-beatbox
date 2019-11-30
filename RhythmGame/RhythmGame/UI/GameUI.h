#pragma once

#include "../Global.h"
#include "../States/Game.h"

#define GAMEUI_OFFSET 8

extern void GameUI_Init(GAMETYPE type);


extern void GameUI_Render();

// Call to decrease hearts
void GameUI_DecreaseHealth(int damage, int which);

// Call to increase score
void GameUI_AddScore(int amt);
