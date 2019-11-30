#pragma once

#include "../Global.h"

#define MAP_OFFSET 5
#define MAP_SHAKE_X 7
#define MAP_SHAKE_Y 4
#define GAME_UI_OFFSET 20
// Initialise the static boundary
extern void Map_Init();

// Call this to render all boundaries
extern void Map_Render();

extern void Map_Update();

// Call to shake map
extern void Map_Shake(DIRECTION dir, double duration, int intensity);
extern int Map_GetShakeFactor(DIRECTION dir);

extern Vector2d Map_GetOrigin();
extern Vector2d Map_GetEnd();
