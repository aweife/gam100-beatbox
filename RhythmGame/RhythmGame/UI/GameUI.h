#pragma once

#include "../Global.h"

#define GAMEUI_OFFSET 8

extern void GameUI_Init();

extern void GameUI_Update();

extern void GameUI_Render();

// Call to decrease hearts
void GameUI_DecreaseHealth(int damage);

// Call to increase score
void GameUI_AddScore(int amt);
