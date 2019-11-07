#include "Map.h"

void Map_Init()
{
	// Initialise offset
	mapOffset = MAP_OFFSET;
	Map_SetOffset(0);
}

void Map_SetOffset(int offset)
{
	// Set mapOrigin at top left corner
	mapOrigin.x = mapOffset - offset;
	mapOrigin.y = mapOffset - offset;

	// Set mapEnd at bottom right corner
	mapEnd.x = GAME_WIDTH - mapOffset + offset;
	mapEnd.y = GAME_HEIGHT - mapOffset + offset;
}

void Map_Render()
{
	for (int i = 0; i < mapEnd.x - mapOrigin.x; i++)
	{
		Console_SetRenderBuffer_CharColor(mapOrigin.x + i, mapOrigin.y, 'X', GREEN);
		Console_SetRenderBuffer_CharColor(mapOrigin.x + i, mapEnd.y, 'X', GREEN);
	}

	for (int i = 0; i < mapEnd.y - mapOrigin.y + 1; i++)
	{
		Console_SetRenderBuffer_CharColor(mapOrigin.x, mapOrigin.y + i, 'X', GREEN);
		Console_SetRenderBuffer_CharColor(mapEnd.x, mapOrigin.y + i, 'X', GREEN);
	}

	for (int i = 0; i < 95 - 5; i++)
	{
		Console_SetRenderBuffer_CharColor(5+ i, 5, 'X', DARKCYAN);
		Console_SetRenderBuffer_CharColor(5 + i, 75, 'X', DARKCYAN);
	}

	for (int i = 0; i < 75 - 5; i++)
	{
		Console_SetRenderBuffer_CharColor(5, 5 + i, 'X', DARKCYAN);
		Console_SetRenderBuffer_CharColor(95, 5 + i, 'X', DARKCYAN);
	}

	
	
}