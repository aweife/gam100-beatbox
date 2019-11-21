#pragma once

#define SPRITE_SIZE 4500
#include "../Global.h"

typedef struct sprite
{
	Vector2d origin;
	Vector2d position[SPRITE_SIZE];
	char printchar[SPRITE_SIZE];
	CONSOLECOLOR printColor[SPRITE_SIZE];
	int charCount;
}sprite;

//initialise for skullenemy
void Text_Init(sprite* s, char* path);

//creates a skullenemy
sprite Text_CreateSprite();

//Renders enemy
void Text_Render(sprite *s, int offsetX, int offsetY);

// Set enemy position
void Text_Move(sprite *s, int x, int y);

//Read from text file and store in array
void _Readandstoretext(sprite *s, const char *path);

