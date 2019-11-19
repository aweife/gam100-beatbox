#pragma once

#include "../Global.h"

#define MAP_OFFSET 20

// Initialise the static boundary
extern void Map_Init();

// Call this to render all boundaries
extern void Map_Render();

extern void Map_Update();

// Call to shake map
extern void Map_Shake(double duration, int intensity);
extern int Map_GetShakeFactor();