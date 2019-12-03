#pragma once

#include "../Global.h"

#define MAP_OFFSET 5
#define MAP_SHAKE_X 5
#define MAP_SHAKE_Y 3
#define GAME_UI_OFFSET 20

// Initialise the static boundary
extern void Map_Init(int offset);

// Call this to render all boundaries
extern void Map_Render();

// Only for updating the shaking of map
extern void Map_Update();

// Call to shake map
extern void Map_Shake(DIRECTION dir, double duration, int intensity);

// To update other files if the map is shaking
extern int Map_GetShakeFactor(DIRECTION dir);

// To let other files know the starting x,y of the stationary border
extern Vector2d Map_GetOrigin();

// To let other files know the end x,y of the stationary border
extern Vector2d Map_GetEnd();
