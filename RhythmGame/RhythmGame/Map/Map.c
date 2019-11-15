#include "Map.h"
#include "../Audio/AudioEngine.h"
#include "../Console/Console.h"
#include "../Global.h"

// Static boundary
static Vector2d sMapOrigin;
static Vector2d sMapEnd;
static int sMapOffset = MAP_OFFSET;

// Private functions
void _CreateStatic();
void Map_Visualise(int offset, CONSOLECOLOR color);

void Map_Init()
{
	// Initialise static boundary
	sMapOrigin.x = sMapOffset;
	sMapOrigin.y = sMapOffset;
	sMapEnd.x = GAME_WIDTH - sMapOffset;
	sMapEnd.y = GAME_HEIGHT - sMapOffset;
}

void Map_Update()
{

}

void Map_Render()
{
	_CreateStatic();
}

void _CreateStatic()
{
	// Horizontal
	for (int i = 0; i < sMapEnd.x - sMapOrigin.x; i++)
	{
		Console_SetRenderBuffer_CharColor(sMapOrigin.x + i, sMapOrigin.y, 'X', DARKCYAN);
		Console_SetRenderBuffer_CharColor(sMapOrigin.x + i, sMapEnd.y, 'X', DARKCYAN);
	}

	// Vertical
	for (int i = 0; i < sMapEnd.y - sMapOrigin.y; i++)
	{
		Console_SetRenderBuffer_CharColor(sMapOrigin.x, sMapOrigin.y + i, 'X', DARKCYAN);
		Console_SetRenderBuffer_CharColor(sMapEnd.x, sMapOrigin.y + i, 'X', DARKCYAN);
	}
}

void Map_Visualise(int offset, CONSOLECOLOR color)
{
	// Horizontal
	for (int i = 0; i < (sMapEnd.x+offset) - (sMapOrigin.x-offset); i++)
	{
		Console_SetRenderBuffer_CharColor((sMapOrigin.x - offset) + i, (sMapOrigin.y - offset), 'X', color);
		Console_SetRenderBuffer_CharColor((sMapOrigin.x - offset) + i, (sMapEnd.y + offset), 'X', color);
	}

	// Vertical
	for (int i = 0; i < (sMapEnd.y + offset) - (sMapOrigin.y - offset); i++)
	{
		Console_SetRenderBuffer_CharColor((sMapOrigin.x - offset), (sMapOrigin.y - offset) + i, 'X', color);
		Console_SetRenderBuffer_CharColor((sMapEnd.x + offset), (sMapOrigin.y - offset) + i, 'X', color);
	}
}