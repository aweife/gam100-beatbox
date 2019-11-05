#pragma once

#include "../Clock/Clock.h"

#define boxSize 3

typedef enum DIRECTION
{
	TOPLEFT = 1,
	TOPRIGHT = 2,
	BOTTOMRIGHT = 3,
	BOTTOMLEFT = 4,
	UP = 5,
	RIGHT = 6,
	DOWN = 7,
	LEFT = 8,
	STAY = 0,
}Direction;

typedef enum EaseMovement
{
	SpeedUp = 0,
	SlowDown = 1,
	Default = 2,
}EaseMovement;

typedef struct Player
{
	int originX;
	int originY;
	int playerX[boxSize * boxSize];
	int playerY[boxSize * boxSize];
}Player;

static double pPosX;
static double pPosY;
int direction;
static double dt;
static double velocity;

static double factor;
static double EaseTimer;
static int EaseBool;
static int EaseCheck;

// Initialise player variables
void playerInit();

// Moves the player according to input
void playerMove(int direction);

void diagonalCheck();
void playerShape();
Player* playerGetPosition();