#pragma once

#include "../Functions.h"

#define MAP_OFFSET 5

coords mapOrigin;
coords mapEnd;

int mapOffset;

extern void Map_Init();

extern void Map_SetOffset(int offset);

extern void Map_Render();