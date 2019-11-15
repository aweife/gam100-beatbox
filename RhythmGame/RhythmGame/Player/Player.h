#pragma once

#include "../Global.h"

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

typedef enum EASEMOVEMENT {
	SpeedUp = 0,
	SlowDown = 1,
	Default = 2,
}EASEMOVEMENT;

typedef struct Player {
	coords position;
	coords body[BOXSIZE * BOXSIZE];
	int direction;
	int health;
}Player;

// Initialise player variables
void Player_Init();

// Moves the player according to input (WITH DASH MECHANISM)
void Player_Update();

// Renders the player location
void Player_Render();

// Allows player to dash
void Player_Dash();

// Damages Player
void Player_Damage();

// Sets direction and EaseMovement to allow movement in direction with Ease In Ease Out effect
extern void Player_SetVel(DIRECTION dir, EASEMOVEMENT EaseC);

// Returns value of factor
double Player_GetEaseFactor();

// Returns direction of player
int Player_GetDirection();