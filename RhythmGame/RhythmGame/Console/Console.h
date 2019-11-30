/********************************************************************************** 
* \file			Console.h
* \brief		A Wrapper to windows.h, to simplify a Console application usage
* \author		Yannick Gerber
* \version		1.2
* \date			2019
* 
*	The Console library Wraps around the windows library to expose comprehensive
*	accessors and functions for a Windows Console application
*	
*	
* \note			Course: GAM100
* \copyright	Copyright (c) 2019 DigiPen Institute of Technology. Reproduction 
				or disclosure of this file or its contents without the prior 
				written consent of DigiPen Institute of Technology is prohibited.
*
* \Revisions	1.2 - Added Palette Color handling
*					- Added ways to modify a character foreground and background colors
*					- Added More Options for Font selection
*					- Removed the API test function to remove warnings
*					- Fixed the SetConsoleTitle on UNICODE enabled systems
*					- Added a String rendering that accept variadic arguments
*					- Fixed a issue with the buffer access that would lead to a 
*					crash when requesting an out of index address
*					
*				1.1 - Fixed a windows size inconsistency issue
*				
*				1.0 - Initial commit
**********************************************************************************/
#ifndef CONSOLE_H
#define CONSOLE_H
#include <Windows.h>
#include <stdbool.h>


/**
 * \brief	Initializes the Library, needs to be called first
* \note	A color is an unsigned long, 4 bytes, ARGB
* \note - A : Alpha(not used)
* \note - R : Red
* \note - G : Green
* \note - B : Blue
* \note	examples : 0x00FFFFFF is White(R = 255, G = 255, B = 255)
* \note			  0x00000000 is Black(R = 0, G = 0, B = 0)
* \note			  0x00FF0000 is Red(R = 255, G = 0, B = 0)
* \note			  0x0000FF00 is Green(R = 0, G = 255, B = 0)
* \note			  0x0000FF00 is Blue(R = 0, G = 255, B = 0)
*/
typedef struct
{
	unsigned long color[16];
} ConsoleColorPalette;

//*********************************************************************************
//						CONSOLE INITIALIZATION
//*********************************************************************************

/**
 * \brief	Initializes the Library, needs to be called first	
 * \note	Do not forget to call Console_CleanUp() at the end 
 */
void Console_Init();


/**
 * \brief	Set the title of your Console Window
 * \param	title ANSI style String
 */
void Console_SetTitle(const char* title);


/**
 * \brief	Set the font to a Squared Ratio font
 * 
 * \note	Uses the Terminal Font, in 8x8 resolution
 */
void Console_SetSquareFont();


/**
 * \brief	Set the font to the Terminal font with specified Width and Height
 *
 * \note	Valid Width and height : 4x6, 6x8, 8x8, 16x8, 5x12, 7x12, 8x12, 16x12, 12x16, 10x18
 *			Other combinations will not work
 */
void Console_SetTerminalFont(int width, int height);


/**
 * \brief	Get the console resolution Ratio
 * \return	Ratio in float
 */
float Console_GetDisplayRatio();


/**
 * \brief	Move the Window to the specified position
 * \param	x 
 * \param	y 
 */
void Console_SetWindowPos(int x, int y);


/**
 * \brief	Set the Console to full screen mode
 *
 * Set the Window to a real full screen, without Header and buttons
 */
void Console_SetFullScreen();


/**
 * \brief Set the Console in Window Mode
 * \param sizeX Number of Rows
 * \param sizeY Number of Columns
 * \param centeredWindow true = Center the Window in the Screen, false = set position at 0,0
 */
void Console_SetWindowedMode(int sizeX, int sizeY, bool centeredWindow);


/**
 * \brief	Set the console to Windowed full Screen mode
 *
 * Maximize the console to the full size of the screen. The windows bar and command buttons are available.
 */
void Console_SetWindowedFullScreen();

/**
 * \brief	Set the size of the console
 * \param	x Column
 * \param	y Row
 *
 * Will have no effect in Full Screen
 */
void Console_SetSize(int x, int y);


/**
 * \brief	Returns the current Height of the Console
 * \return	Height of the Console
 */
int Console_GetHeight();


/**
 * \brief	Returns the current Width of the Console
 * \return	Width of the Console
 */
int Console_GetWidth();


/**
 * \brief	Set the Console Cursor visibility
 * \param	visible 0 = Invisible, 1 = Visible
 */
void Console_SetCursorVisibility(int visible);

/**
 * \brief	Simple test run to ensure the Library is working
 */
void Console_TestAPI();

//*********************************************************************************
//						SIMPLE TEXT DISPLAY FUNCTION
//*********************************************************************************

/**
 * \brief	Move the Cursor of the console Buffer to the specified position
 * \param	x Column
 * \param	y Row
 * \note	This function moves the cursor, the next printf_s(), or putchar(), will start from the cursor
 * \note	Do not use it with the double buffer system, it only works on the console buffer
 */
void Console_SetCursorPos(int x, int y);

/**
 * \brief	Display a single Character at the specified coordinate in the console Buffer
 * \param	x Columns
 * \param	y Row
 * \param	c Character to display
 *
 * \note	Example: Console_Putchar(20,15,'X');
 * \note	Do not use it with the double buffer system, it only outputs on the console buffer
 */
void Console_Putchar(int x, int y, const char c);

/**
 * \brief	Display a String at the specified coordinate in the console Buffer
 * \param	x Columns
 * \param	y Row
 * \param	s String to display
 *
 * \note	Example: Console_Printf(20,15,"Hello World");
 * \note	Do not use it with the double buffer system, it only outputs on the console buffer
 */
void Console_Printf(int x, int y, const char* s);

/**
 * \brief	Clear the Console Buffer
 * 
 * \note	Do not use it with the double buffer system, it only cleans the console buffer
 */
void Console_Clear();

/**
 * \brief		Clear the specified Area only of the console Buffer
 * \param x		Column, Left position of the area to clear
 * \param y		Row, Top Position of the area to clear
 * \param width Width of the area to clear
 * \param height Height of the area to clear
 * 
 * \note	Do not use it with the double buffer system, it only cleans the console buffer
 */
void Console_ClearArea(int x, int y, int width, int height);


/**
 * \brief Cleanup the console properly
 */
void Console_CleanUp();


//*********************************************************************************
//						DOUBLE BUFFERING SYSTEM
//*********************************************************************************

/**
 * \brief	Display a Character at the specified coordinate in the Back Buffer
 * \param	x Columns
 * \param	y Row
 * \param	c Character to display
 *
 * \note	Example: Console_SetRenderBuffer_Char(20,15,'A');
 * \note	The character will be displayed on your screen only after a Console_SwapRenderBuffer() call
 */
void Console_SetRenderBuffer_Char(int x, int y, char c);


/**
 * \brief	Copies the Console Color palette information to the specified Palette pointer
 * \param	p Console color palette, an array of 16 Color
 *
 * \note	A color is an unsigned long, 4 bytes, ARGB
 */
void Console_GetColorPalette(ConsoleColorPalette* p);

/**
 * \brief	Copies the specified Palette color to the Console 
 * \param	p Console color palette, an array of 16 Color
 *
 * \note	A color is an unsigned long, 4 bytes, ARGB
 */
void Console_SetColorPalette(ConsoleColorPalette* p);

/**
 * \brief	Sets a Cell at the specified coordinate in the Back Buffer
 * \param	x Columns
 * \param	y Row
 * \param	c Character
 * \param	fgColor Foreground color, color index of the palette
 * \param	bgColor Background color, color index of the palette
 *
 * \note	Example: Console_SetRenderBuffer_Cell(20,15,'A', 3, 10);
 * \note	will display A in the coordinate 20, 15 cell, with the character color being the 3rd in the
 * \note	current color palette, and the 10th color as a background color
 * \note	The character will be displayed on your screen only after a Console_SwapRenderBuffer() call
 */
void Console_SetRenderBuffer_Cell(int x, int y, char c, unsigned short fgColor, unsigned short bgColor);

/**
 * \brief	Sets a Cell at the specified coordinate in the Back Buffer
 * \param	x Columns
 * \param	y Row
 * \param	c Character
 *
 * \note	Example: Console_SetRenderBuffer_CellChar(20,15,'A');
 * \note	will display A in the coordinate 20, 15 cell
 * \note	The character will be displayed on your screen only after a Console_SwapRenderBuffer() call
 */
void Console_SetRenderBuffer_CellChar(int x, int y, char c);

/**
 * \brief	Sets a Cell Color at the specified coordinate in the Back Buffer
 * \param	x Columns
 * \param	y Row
 * \param	color Color indexes
 *
 * \note	Example: Console_SetRenderBuffer_CellChar(20,15, 0xAF);
 * \note	will change the color in the coordinate 20, 15 cell to foreground F and background A index
 * \note	of the color palette
 * \note	The character will be displayed on your screen only after a Console_SwapRenderBuffer() call
 */
void Console_SetRenderBuffer_CellColor(int x, int y, unsigned short color);

/**
 * \brief	Sets a Cell Color at the specified coordinate in the Back Buffer
 * \param	x Columns
 * \param	y Row
 * \param	fgColor Foreground Color
 * \param	bgColor Background Color
 *
 * \note	Example: Console_SetRenderBuffer_CellChar(20,15, 7, 8);
 * \note	will change the color in the coordinate 20, 15 cell to foreground 7 and background 8 index
 * \note	of the color palette
 * \note	The character will be displayed on your screen only after a Console_SwapRenderBuffer() call
 */
void Console_SetRenderBuffer_CellColors(int x, int y, unsigned short fgColor, unsigned short bgColor);

/**
 * \brief	Display a String at the specified coordinate in the Back Buffer
 * \param	x Columns
 * \param	y Row
 * \param	s String to display
 *
 * \note	Example: Console_SetRenderBuffer_String(20,15,"Hello digipen");
 * \note	The String will be displayed on your screen only after a Console_SwapRenderBuffer() call
 */
void Console_SetRenderBuffer_String(int x, int y, const char* s);


/**
 * \brief	Display a formatted string at the specified coordinate in the Back Buffer
 * \param	x Columns
 * \param	y Row
 * \param	format String format to display
 * \param	... arguments to use in the formatting
 *
 * \note	Example: Console_SetRenderBuffer_Printf(20,15,"Decimal Number is %d, Floating is %f", d, f);
 * \note	The String will be displayed on your screen only after a Console_SwapRenderBuffer() call
 */
void Console_SetRenderBuffer_Printf(int x, int y, const char* format, ...);

/**
 * \brief	Clears the Back buffer
 * \note	No effect on your display, you will see the result only after a Console_SwapRenderBuffer() call
 * \note	Should be called at the beginning of your Rendering system
 */
void Console_ClearRenderBuffer();


/**
 * \brief	Copy the Back Buffer information to the Console Buffer
 * 
 * \note	this will display all the changes you calculated during this render pass
 * \note	Should be called at the end of your Rendering system, as your final procedure to bring the render to screen
 */
void Console_SwapRenderBuffer();

/**
 * \brief	Callback prototype for a Render Post Process effect
 */
typedef void (*PostProcessFunctor)(CHAR_INFO*, int);

/**
 * \brief	To be called at anytime during your rendering to 
 *
 * \note	This will call the function passed in parameter, with the renderbuffer pointer and capacity
 */
void Console_RenderBuffer_PostProcess(PostProcessFunctor fct);

#endif // CONSOLE_H
