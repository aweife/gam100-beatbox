#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "Console.h"

//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************

static HANDLE		sConsoleHandle			= 0x00;
static bool			sConsoleInitialized		= false;
static float		sConsoleDisplayRatio	= 4.f / 3.f;
static COORD		sConsoleSize;
static CHAR_INFO*	sRenderBuffer			= 0x00;
static int			sRenderBufferSize		= 0;

static const COORD ORIGIN = { 0,0 };

/**********************************************************************************/


//*********************************************************************************
//								CONTROL FUNCTIONS
//*********************************************************************************

void Console_HandleValidity_Assert()
{
	// Crash here: you haven't called Console_Init() in your code before using the library
	assert(sConsoleHandle != 0x00);
}


//*********************************************************************************
//								RENDER BUFFER HANDLING
//*********************************************************************************

void Console_FreeRenderBuffer()
{
	if (sRenderBuffer == 0)
		return;

	free(sRenderBuffer);
	sRenderBuffer = 0;
	sRenderBufferSize = 0;
}

void Console_CreateRenderBuffer()
{
	if (sRenderBuffer != 0)
		Console_FreeRenderBuffer();

	sRenderBufferSize = Console_GetHeight() * Console_GetWidth() * sizeof(CHAR_INFO);

	sRenderBuffer = malloc(sRenderBufferSize);
}

void Console_ClearRenderBuffer()
{
	if (sRenderBuffer == 0)
		return;

	memset(sRenderBuffer, 0, sRenderBufferSize);
}

void Console_SwapRenderBuffer()
{
	if (sRenderBuffer == 0)
		return;

	// Copy the local rendering buffer to the ConsoleHandle Buffer
	SMALL_RECT writeRegion = { 0,0,sConsoleSize.X, sConsoleSize.Y };

	if (!WriteConsoleOutputA(sConsoleHandle, sRenderBuffer, sConsoleSize, ORIGIN, &writeRegion) )
	{
		DWORD error = GetLastError();
		assert(1);
	}
}

void Console_SetRenderBuffer_Char(int x, int y, char c)
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif

	const int index = x + y * sConsoleSize.X;

	if ((index >= sRenderBufferSize) || (index < 0))
		return;

	sRenderBuffer[index].Char.AsciiChar = c;
	sRenderBuffer[index].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
	// FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
}

void Console_SetRenderBuffer_CharColor(int x, int y, char c, WORD attributes)
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif

	const int index = x + y * sConsoleSize.X;

	if ((index >= sRenderBufferSize) || (index < 0))
		return;

	sRenderBuffer[index].Char.AsciiChar = c;
	sRenderBuffer[index].Attributes = attributes;
	// FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
}

void Console_SetRenderBuffer_String(int x, int y, const char* s)
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif
	const int size = strlen(s);
	int index = x + y * sConsoleSize.X;
	for (int i = 0; i < size; ++i)
	{
		if ((index >= sRenderBufferSize) || (index < 0))
			return;

		sRenderBuffer[index].Char.AsciiChar = s[i];
		sRenderBuffer[index].Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
		++index;
	}
}

//*********************************************************************************
//								CONSOLE & WINDOW
//*********************************************************************************

void Console_Init()
{
	sConsoleInitialized = true;

	// Main buffer in normal mode, Backbuffer in double buffer
	sConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Disable the Mouse quick edit mode for the console
	HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD inputFlags;
	GetConsoleMode(inputHandle, &inputFlags);
	SetConsoleMode(inputHandle, ENABLE_EXTENDED_FLAGS | (inputFlags& ~ENABLE_QUICK_EDIT_MODE));

	// Get the Current Font Ratio
	CONSOLE_FONT_INFO fontInfo;
	GetCurrentConsoleFont(sConsoleHandle, FALSE, &fontInfo);
	sConsoleDisplayRatio = fontInfo.dwFontSize.X / (float)fontInfo.dwFontSize.Y;
}

void Console_SetSquareFont()
{
	// Flattens the font, Terminal and 8x8
	CONSOLE_FONT_INFOEX fontInfo = { 0 };
	fontInfo.cbSize = sizeof(fontInfo);
	wcscpy_s(fontInfo.FaceName, _countof(fontInfo.FaceName), L"Terminal");
	fontInfo.dwFontSize.X = 8;
	fontInfo.dwFontSize.Y = 8;
	fontInfo.FontWeight = FW_NORMAL;
	sConsoleDisplayRatio = fontInfo.dwFontSize.X / (float)fontInfo.dwFontSize.Y;
	
	SetCurrentConsoleFontEx(sConsoleHandle, FALSE, &fontInfo);
}

float Console_GetDisplayRatio()
{
	return sConsoleDisplayRatio;
}

void Console_SetTitle(const char* title)
{
	// Lazy conversion from ANSI to Unicode, Limited to 100 character
	// use dynamic memory allocation and not a fixed array to remove the 100 character limitation
	wchar_t  lTitle[100];
	swprintf_s(lTitle, 100, L"%hs", title);

	SetConsoleTitle(lTitle);
}

void Console_SetSize(int x, int y)
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif

	const COORD size = { x, y };
	const SMALL_RECT windowSize = { 0, 0, size.X - 1, size.Y - 1 };
	
	// HACK: sometimes the windows is not of the asked size, because the Buffer can never be smaller than the windows
	// FIX: Force a small windows, set buffer, and resize windows to the correct size
	const SMALL_RECT smallWindowSize = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(sConsoleHandle, 1, &smallWindowSize);
	// END HACK

	SetConsoleScreenBufferSize(sConsoleHandle, size);
	SetConsoleWindowInfo(sConsoleHandle, 1, &windowSize);

	sConsoleSize = size;
}

void Console_SetFullScreen()
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif

	CONSOLE_SCREEN_BUFFER_INFO  screenBufferInfo;

	// Set the console to full Screen and changes its buffer
	SetConsoleDisplayMode(sConsoleHandle, CONSOLE_FULLSCREEN_MODE, NULL);
	GetConsoleScreenBufferInfo(sConsoleHandle, &screenBufferInfo);
	SetConsoleScreenBufferSize(sConsoleHandle, screenBufferInfo.dwMaximumWindowSize);
	sConsoleSize = screenBufferInfo.dwMaximumWindowSize;

	// Create the rendering buffer
	Console_CreateRenderBuffer();

	// Bring the window to the top
	BringWindowToTop(GetConsoleWindow());
}

void Console_SetWindowPos(int x, int y)
{
	SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, x, y, 0, 0, SWP_FRAMECHANGED | SWP_DRAWFRAME | SWP_NOSIZE);
}

void Console_SetWindowedMode(int sizeX, int sizeY, bool centeredWindow)
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif

	SetConsoleDisplayMode(sConsoleHandle, CONSOLE_WINDOWED_MODE, NULL);
	Console_SetSize(sizeX, sizeY);

	// Center the window on screen
	const HWND   consoleHWND = GetConsoleWindow();
	RECT   windowsRect;
	RECT   consoleRect;

	GetWindowRect(consoleHWND, &consoleRect);
	GetWindowRect(GetDesktopWindow(), &windowsRect);

	// Create the rendering buffer
	Console_CreateRenderBuffer();

	// Bring it on top
	BringWindowToTop(consoleHWND);

	// Removes the resize ability
	SetWindowLongPtr(consoleHWND, GWL_STYLE, GetWindowLongPtr(consoleHWND, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	// Set the Window to the center
	if (centeredWindow)
	{
		const int newX = ((windowsRect.right - windowsRect.left) / 2 - (consoleRect.right - consoleRect.left) / 2);
		const int newY = ((windowsRect.bottom - windowsRect.top) / 2 - (consoleRect.bottom - consoleRect.top) / 2);
		Console_SetWindowPos(newX, newY);
	}
	else
		Console_SetWindowPos(0, 0);
}


void Console_SetWindowedFullScreen()
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif

	const COORD maxSize = GetLargestConsoleWindowSize(sConsoleHandle);
	Console_SetWindowedMode(maxSize.X, maxSize.Y, false);
}

int Console_GetWidth()
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif

	return sConsoleSize.X;
}

int Console_GetHeight()
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif

	return sConsoleSize.Y;
}

void Console_SetCursorVisibility(int visible)
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(sConsoleHandle, &cursorInfo);
	cursorInfo.bVisible = (visible == 0) ? false : true;
	SetConsoleCursorInfo(sConsoleHandle, &cursorInfo);
}

//*********************************************************************************
//			STANDARD USAGE OF CONSOLE : Clear / Print / Putchar
//*********************************************************************************

void Console_SetCursorPos(int x, int y)
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif

	const COORD pos = {x, y};
	SetConsoleCursorPosition(sConsoleHandle, pos);
}

// If you use printF
COORD Console_GetCursorPos()
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif

	CONSOLE_SCREEN_BUFFER_INFO  screenBufferInfo;
	GetConsoleScreenBufferInfo(sConsoleHandle, &screenBufferInfo);

	return screenBufferInfo.dwCursorPosition;
}

void Console_Putchar(int x, int y, const char s)
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif

	Console_SetCursorPos(x, y);
	putchar(s);
}

void Console_Printf(int x, int y, const char* s)
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif

	Console_SetCursorPos(x, y);
	printf_s(s);
}

void Console_Clear()
{
#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif
	// Fills the whole console with a space character
	// does not change the color of a cell, use FillConsoleOutputAttribute to clean out properly the colors if you decide to use colors
	CONSOLE_SCREEN_BUFFER_INFO screenInfoBuffer;
	GetConsoleScreenBufferInfo(sConsoleHandle, &screenInfoBuffer);

	const COORD firstCell = { 0, 0 };
	const int cellCount = screenInfoBuffer.dwSize.X *screenInfoBuffer.dwSize.Y;
	DWORD writtenCells = 0;
	FillConsoleOutputCharacter(sConsoleHandle, ' ', cellCount, firstCell, &writtenCells);
}

void Console_ClearArea(int x, int y, int width, int height)
{

#ifdef _DEBUG
	Console_HandleValidity_Assert();
#endif

	char buffer[512];
	memset(buffer, ' ', width);
	buffer[width] = '\0';

	for (int j = y; j < y + height; ++j)
		Console_Printf(x, j, buffer);
}

void Console_TestAPI()
{
	Console_SetTitle("Test API");
	printf_s("Hello World!\n");
	getchar();

	Console_SetFullScreen();
	Console_SetCursorVisibility(0);
	getchar();

	Console_Clear();
	getchar();

	Console_SetCursorPos(50, 25);
	printf_s("Hello Digipen !~ ");

	Console_SetRenderBuffer_Char(60, 27, 'A');
	Console_SetRenderBuffer_Char(5, 27, 'B');

	printf_s("How is it going ?! ");

	Console_SetCursorPos(0, 28);
	printf_s("Screen size: %d x %d !\n", Console_GetWidth(), Console_GetHeight());
	getchar();

	Console_SetWindowedMode(100,20, true);
	printf_s("Screen size: %d x %d !\n", Console_GetWidth(), Console_GetHeight());
	getchar();

	Console_SetSize(120, 50);
	printf_s("Screen size: %d x %d !\n", Console_GetWidth(), Console_GetHeight());
	getchar();

	Console_SetWindowedFullScreen();
	printf_s("Screen size: %d x %d !\n", Console_GetWidth(), Console_GetHeight());
	getchar();
}


//*********************************************************************************
//									CLEAN UP
//*********************************************************************************

void Console_CleanUp()
{
	Console_FreeRenderBuffer();
	Console_Clear();
	DestroyWindow(GetConsoleWindow());
}
