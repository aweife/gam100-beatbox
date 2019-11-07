#pragma once

#include "../Functions.h"
#include "../Audio/AudioEngine.h"

#define MAP_OFFSET 5

// Initialise the static boundary
extern void Map_Init();

// Call this to render all boundaries
extern void Map_Render();