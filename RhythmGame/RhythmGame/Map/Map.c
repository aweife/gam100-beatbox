#include "Map.h"
#include "../Audio/AudioEngine.h"
#include "../Console/Console.h"
#include "../Global.h"
#include "../Beat/Beat.h"
#include "../Clock/Clock.h"
#include "../Random/Random.h"

// Static boundary
static Vector2d MapOrigin;
static Vector2d MapEnd;
static int MapOffset = MAP_OFFSET;

// For map shake
static double shakeDuration = 0;
static double shakeTimer = 0;
static int shakeCount = 0;
static int shakeFactor = 0;

// Private functions
void _CreateStatic();
void _CreateVisualiser(int offset, CONSOLECOLOR color);
void _ShakeMap();

void Map_Init()
{
	// Initialise static boundary
	MapOrigin.x = MapOffset;
	MapOrigin.y = MapOffset;
	MapEnd.x = GAME_WIDTH - MapOffset;
	MapEnd.y = GAME_HEIGHT - MapOffset;

	// Map shake
	shakeDuration = 0;
	shakeFactor = 0;
}

void Map_Update()
{
	if (shakeCount > 0)
		_ShakeMap();
}

void Map_Render()
{
	_CreateStatic();
	if (Beat_GetKick())
		_CreateVisualiser(2, GREEN);
	if (Beat_GetSnare())
		_CreateVisualiser(4, RED);
}

void Map_Shake(double duration, int intensity)
{
	shakeDuration = duration;
	shakeCount = intensity;
	shakeFactor = shakeCount;
}

int Map_GetShakeFactor()
{
	return shakeFactor % 2;
}

void _CreateStatic()
{
	// Horizontal
	for (int i = 0; i < MapEnd.x - MapOrigin.x; i++)
	{
		Console_SetRenderBuffer_CharColor(MapOrigin.x + i + 1, MapOrigin.y, 'X', DARKCYAN);
		Console_SetRenderBuffer_CharColor(MapOrigin.x + i, MapEnd.y, 'X', DARKCYAN);
	}

	// Vertical
	for (int i = 0; i < MapEnd.y - MapOrigin.y; i++)
	{
		Console_SetRenderBuffer_CharColor(MapOrigin.x, MapOrigin.y + i, 'X', DARKCYAN);
		Console_SetRenderBuffer_CharColor(MapEnd.x, MapOrigin.y + i + 1, 'X', DARKCYAN);
	}
}

void _CreateVisualiser(int offset, CONSOLECOLOR color)
{
	// Horizontal
	for (int i = 0; i < (MapEnd.x + offset) - (MapOrigin.x - offset); i++)
	{
		Console_SetRenderBuffer_CharColor((MapOrigin.x - offset) + i + 1, (MapOrigin.y - offset), 'X', color);
		Console_SetRenderBuffer_CharColor((MapOrigin.x - offset) + i, (MapEnd.y + offset), 'X', color);
	}

	// Vertical
	for (int i = 0; i < (MapEnd.y + offset) - (MapOrigin.y - offset); i++)
	{
		Console_SetRenderBuffer_CharColor((MapOrigin.x - offset), (MapOrigin.y - offset) + i, 'X', color);
		Console_SetRenderBuffer_CharColor((MapEnd.x + offset), (MapOrigin.y - offset) + i + 1, 'X', color);
	}
}

void _ShakeMap()
{
	shakeTimer += Clock_GetDeltaTime();

	if (shakeTimer > (double)(shakeDuration/10))
	{
		shakeTimer = 0.0;
		shakeFactor *= -1;
		shakeCount--;

		// Shake map
		MapOrigin.x += shakeFactor;
		MapEnd.x += shakeFactor;
	}

	if (!shakeCount)
	{
		shakeFactor = 0;
		MapOrigin.x = MapOffset;
		MapEnd.x = GAME_WIDTH - MapOffset;
	}
}