#pragma once

#include <Windows.h>

#define GAME_WIDTH 200
#define GAME_HEIGHT 150

typedef enum CONSOLECOLOR {
	BLACK = 0,
	DARKBLUE = FOREGROUND_BLUE,
	DARKGREEN = FOREGROUND_GREEN,
	DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
	DARKRED = FOREGROUND_RED,
	DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
	DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
	DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	GRAY = FOREGROUND_INTENSITY,
	BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
	MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
	WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	bRED = BACKGROUND_RED,
	bBLUE = BACKGROUND_BLUE,
} CONSOLECOLOR;

typedef struct Vector2d {
	int x;
	int y;
	double eulerX;
	double eulerY;
} Vector2d;
