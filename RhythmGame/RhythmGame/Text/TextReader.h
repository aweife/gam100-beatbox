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

//Read from text file and store in array
void _Readandstoretext(sprite *s, const char *path);

//Render each line according to rainbow colour
void Text_RenderRainbow(sprite* s);

//stores text in arrays
void _ReadandstoretextArray(sprite* s, const char* path,int state);

//Render words
void Text_RenderWords(sprite* s);

//free memory allocated to spriteInfo
void Text_Cleanup(sprite* s);

//counts the amount of characters so that Text_Init can malloc accordingly
int _CountChars(char* path);
