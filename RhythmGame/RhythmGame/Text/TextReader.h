#pragma once
#include "../Global.h"

//struct to keep track of the positioning,char and colour of the text.
typedef struct spriteInfo
{
	Vector2d position;
	char printchar;
	CONSOLECOLOR printColor;
}spriteInfo;

//struct with all the information of a sprite to do collisions check and more
typedef struct sprite
{
	Vector2d origin;
	spriteInfo *spriteI;
	int charCount;
}sprite;

//initialise sprite with the specified text file
void Text_Init(sprite* s, char* path);

//Initialise sprite with a specified state
void Text_InitArray(sprite* s, char* path,int state);

//creates a sprite
sprite Text_CreateSprite();

//Renders sprite
void Text_Render(sprite *s, int offsetX, int offsetY);

// Set the sprite's position
void Text_Move(sprite *s, int x, int y);

//Render each line according to rainbow colour
void Text_RenderRainbow(sprite* s);

//Render white words only with foreground colors
void Text_RenderWords(sprite* s);

//free the memory allocated to spriteI of sprites
void Text_Cleanup(sprite* s);

//renders everything with 1 colour specified
void Text_RenderColor(sprite *s, CONSOLECOLOR cc, int offsetX, int offsetY);

