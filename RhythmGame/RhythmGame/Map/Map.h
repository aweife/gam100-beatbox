#pragma once

#include "../Global.h"

#define MAP_OFFSET 5

// Initialise the static boundary
extern void Map_Init();

// Call this to render all boundaries
extern void Map_Render();

// Call to visualise audio
extern void Map_Visualise(int offset, CONSOLECOLOR color);