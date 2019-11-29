#pragma once

#define SPRITE_SIZE 10000

#include "../Global.h"

typedef struct spriteInfo
{
	Vector2d position;
	char printchar;
	CONSOLECOLOR printColor;
}spriteInfo;

typedef struct sprite
{
	Vector2d origin;
	spriteInfo *spriteI;
	int charCount;
}sprite;

//initialise sprite
void Text_Init(sprite* s, char* path);
//Initialise sprite with array
void Text_InitArray(sprite* s, char* path,int state);

//creates a skullenemy
sprite Text_CreateSprite();

//Renders enemy
void Text_Render(sprite *s, int offsetX, int offsetY);

// Set enemy position
void Text_Move(sprite *s, int x, int y);


//Render each line according to rainbow colour
void Text_RenderRainbow(sprite* s);


//Render words
void Text_RenderWords(sprite* s);

//free memory allocated to spriteInfo
void Text_Cleanup(sprite* s);

//renders everything with 1 colour specified
void Text_RenderColor(sprite *s, CONSOLECOLOR cc, int offsetX, int offsetY);

