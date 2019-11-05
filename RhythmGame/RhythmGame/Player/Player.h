#pragma once
#include "../Functions.h"

#define boxSize 3
#define velocity 0.04f

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
	int direction;
	int eulerX;
	int eulerY;
}Player;

static double factor;
static int EaseBool;
static int EaseCheck;
static double EaseTimer;

// Initialise player variables
void playerInit();

// Moves the player according to input
void p_playerMove();
void _playerShape();
Player* playerGetInfo();
double _playerGetEaseFactor();
int _playerGetDirection();

void p_Render();