#pragma once

#include <stdbool.h>
#include "../States/Game.h"
#include <stdio.h>
#include "../Clock/Clock.h"
#include "../Console/Console.h"
#include "../Player/Player.h"

#define BUFFER_SIZE 100
#define SIZE 30

/*typedef struct {
	char name[20];
	int score;
} Player;*/

extern void _RenderScoreBoard(int score, int x, int y);
extern void _InputScore(char* path);
extern void _OutputScore(char* path, int x, int y);