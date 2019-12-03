/*******************************************************************************
* @filename Map
* @author(s) Ang Wei Feng (Ted)
* @DP emails weifeng.ang@digipen.edu
* Brief Description:
* This file creates the border and update according to audio
*******************************************************************************/

#include "Map.h"
#include "../Audio/AudioEngine.h"
#include "../Console/Console.h"
#include "../Global.h"
#include "../Clock/Clock.h"
#include "../Random/Random.h"

// Static boundary
static Vector2d MapOrigin;
static Vector2d MapEnd;
static int uiOffset;

// For map shake
static double shakeDuration = 0;
static double shakeTimer = 0;
static int shakeCount = 0;
static int shakeFactorX = 0;
static int shakeFactorY = 0;

/* Private functions **********************************************************/

// Create a stationary border
void _CreateStatic();

// Create an outer border with size offset
void _CreateVisualiser(int offset, CONSOLECOLOR color);

// Create an faint background of '.'
void _CreateBackground(int offset, CONSOLECOLOR color);

// Offset the borders by an small amount
void _ShakeMap();

// Resets the borders to their original positions
void _ResetMap();

/* Definitions ****************************************************************/

void Map_Init(int offset)
{
	uiOffset = offset;
	// Init Map
	_ResetMap();

	// Map shake
	shakeDuration = 0;
	shakeFactorX = 0;
	shakeFactorY = 0;
}

void Map_Update()
{
	if (shakeCount > 0)
		_ShakeMap();
}

void Map_Render()
{
	if (Audio_GetSpectrum(2))
	{
		_CreateVisualiser(2, GREEN);
		_CreateBackground(2, fDARKGREEN);
	}
	if (Audio_GetSpectrum(1))
	{
		_CreateVisualiser(4, YELLOW);
		_CreateBackground(4, fDARKYELLOW);
	}
	if (Audio_GetSpectrum(0))
	{
		_CreateVisualiser(6, RED);
		_CreateBackground(6, fDARKRED);
	}
	// Render blue last
	_CreateStatic();
}

void Map_Shake(DIRECTION dir, double duration, int intensity)
{
	if (shakeCount > 0) return; 
	shakeDuration = duration;
	shakeCount = intensity;
	if(dir == UP)
		shakeFactorY = shakeCount;
	else
		shakeFactorX = shakeCount;
}

int Map_GetShakeFactor(DIRECTION dir)
{
	if(dir == UP)
		return shakeFactorY;
	else
		return shakeFactorX;
}

Vector2d Map_GetOrigin()
{
	return MapOrigin;
}

Vector2d Map_GetEnd()
{
	return MapEnd;
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

void _CreateBackground(int offset, CONSOLECOLOR color)
{
	for (int i = 1; i < (MapEnd.x + offset) - (MapOrigin.x - offset); i+=4)
		for (int j = 1; j < (MapEnd.y + offset) - (MapOrigin.y - offset); j+=4)
			Console_SetRenderBuffer_CharColor(((MapOrigin.x - offset) + i), ((MapOrigin.y - offset) + j), '.', color);
}

void _ShakeMap()
{
	shakeTimer += Clock_GetDeltaTime();

	if (shakeTimer > shakeDuration)
	{
		shakeTimer = 0.0;
		shakeFactorX *= -1;
		shakeFactorY *= -1;
		shakeCount--;

		// Shake map
		MapOrigin.x += shakeFactorX;
		MapEnd.x += shakeFactorX;

		MapOrigin.y += shakeFactorY;
		MapEnd.y += shakeFactorY;
	}

	if (shakeCount < 1)
	{
		shakeFactorX = 0;
		shakeFactorY = 0;
		_ResetMap();
	}
}

void _ResetMap()
{
	// Initialise static boundary
	MapOrigin.x = MAP_OFFSET + MAP_SHAKE_X;
	MapOrigin.y = MAP_OFFSET + uiOffset + MAP_SHAKE_Y;
	MapEnd.x = GAME_WIDTH - MAP_OFFSET - MAP_SHAKE_X;
	MapEnd.y = GAME_HEIGHT - MAP_OFFSET - MAP_SHAKE_Y;
}