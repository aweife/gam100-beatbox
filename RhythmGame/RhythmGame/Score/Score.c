#define _CRT_SECURE_NO_DEPRECATE

#include "Score.h"
#include <stdio.h>
#include "../Player/Player.h"
#include "../Clock/Clock.h"

FILE* scoreboardFile;
char scoreDisplay[10];

void _RenderScoreBoard(int score)
{
	_itoa_s(score, scoreDisplay, sizeof(scoreDisplay), 10);
	Console_SetRenderBuffer_String(53, 10, "Score:");
	Console_SetRenderBuffer_String(60, 10, scoreDisplay);
}

//write to file
scoreboardFile = fopen("..//RhythmGame//$Resources//Score.txt", "r");


