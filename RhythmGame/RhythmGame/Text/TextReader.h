#pragma once

#define SPRITE_SIZE 1500
#include "../Global.h"

typedef struct sprite
{
	Vector2d origin;
	Vector2d position[SPRITE_SIZE];
	char printchar[SPRITE_SIZE];
	CONSOLECOLOR printColor[SPRITE_SIZE];
	int charCount;
}sprite;

//initialise sprite
void Text_Init(sprite* s, char* path);
//Initialise sprite with array
void Text_InitArray(sprite* s, char* path);

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
void _ReadandstoretextArray(sprite* s, const char* path);

void Text_RenderSprite(sprite* s);