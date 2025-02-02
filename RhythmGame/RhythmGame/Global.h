#pragma once

#include <Windows.h>

#define GAME_WIDTH 200
#define GAME_HEIGHT 150

typedef enum CONSOLECOLOR {
	BLACK = 0,
	DARKBLUE = BACKGROUND_BLUE,
	DARKGREEN = BACKGROUND_GREEN,
	DARKCYAN = BACKGROUND_GREEN | BACKGROUND_BLUE,
	DARKRED = BACKGROUND_RED,
	DARKMAGENTA = BACKGROUND_RED | BACKGROUND_BLUE,
	DARKYELLOW = BACKGROUND_RED | BACKGROUND_GREEN,
	LIGHTGRAY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	GRAY = BACKGROUND_INTENSITY,
	BLUE = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
	GREEN = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
	CYAN = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
	RED = BACKGROUND_INTENSITY | BACKGROUND_RED,
	MAGENTA = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
	YELLOW = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
	WHITE = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	fWHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	fBLACK = 0,
	fDARKBLUE = FOREGROUND_BLUE,
	fDARKGREEN = FOREGROUND_GREEN,
	fDARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
	fDARKRED = FOREGROUND_RED,
	fDARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
	fDARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
	fDARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	fGRAY = FOREGROUND_INTENSITY,
	fBLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	fGREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	fCYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	fRED = FOREGROUND_INTENSITY | FOREGROUND_RED,
	fMAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	fYELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
} CONSOLECOLOR;

typedef struct Vector2d {
	int x;
	int y;
	double eulerX;
	double eulerY;
} Vector2d;

typedef enum DIRECTION
{
	TOPLEFT = 1,
	TOPRIGHT = 2,
	BOTTOMRIGHT = 3,
	BOTTOMLEFT = 4,
	UP = 5,
	RIGHT = 6,
	DOWN = 7,
	LEFT = 8,
	STAY = 0,
}DIRECTION;
