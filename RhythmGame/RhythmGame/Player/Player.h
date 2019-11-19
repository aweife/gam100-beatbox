#pragma once

#include "../Global.h"
#include "../Text/TextReader.h"

#define BOXSIZE 3

typedef enum EASEMOVEMENT {
	SpeedUp = 0,
	SlowDown = 1,
	Default = 2,
}EASEMOVEMENT;

typedef struct Player {
	Vector2d position;
	int direction;
	int health;
	sprite PlayerSprite;
}Player;

// Initialise player variables
extern void Player_Init();

// Moves the player according to input (WITH DASH MECHANISM)
extern void Player_Update();

// Renders the player location
extern void Player_Render();

// Allows player to dash
extern void Player_Dash();

// Damages Player
extern void Player_Damage();

// Sets direction and EaseMovement to allow movement in direction with Ease In Ease Out effect
extern void Player_SetVel(DIRECTION dir, EASEMOVEMENT EaseC);

// Returns value of factor
extern double Player_GetEaseFactor();

// Returns direction of player
extern int Player_GetDirection();

// Returns sprite of player
extern sprite *Player_GetSprite();