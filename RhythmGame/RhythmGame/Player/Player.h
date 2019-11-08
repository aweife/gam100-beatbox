#pragma once
#include "../Functions.h"
#include "../Map/Map.h"
#include "../Enemy/Enemy.h"
#include "../Testread.h"

#define BOXSIZE 3

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
}DIRECTION;

typedef enum EASEMOVEMENT
{
	SpeedUp = 0,
	SlowDown = 1,
	Default = 2,
}EASEMOVEMENT;

typedef struct Player
{
	int originX;
	int originY;
	int playerX[BOXSIZE * BOXSIZE];
	int playerY[BOXSIZE * BOXSIZE];
	int direction;
	double eulerX;
	double eulerY;
}Player;

static double factor;
static double dt;
static int EaseBool;
static int EaseCheck;
static double EaseTimer;
static double velocity;
static double cdTimer;
static double dashTimer;

// Initialise player variables
void p_playerInit();

// Moves the player according to input (WITH DASH MECHANISM)
void P_Update();

// Renders the player location
void P_Render();



// Allows player to dash
void _playerDash();

// Sets direction and EaseMovement to allow movement in direction with Ease In Ease Out effect
extern void _playerSetVel(DIRECTION dir, EASEMOVEMENT EaseC);

// Returns value of factor
double _playerGetEaseFactor();

// Returns direction of player
int _playerGetDirection();