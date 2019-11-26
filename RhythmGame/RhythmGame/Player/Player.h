#pragma once

#include "../Global.h"
#include "../Text/TextReader.h"

#define BOXSIZE 3

typedef enum PLAYERSTATE {
	Normal,
	ExDash,
	Dash,
	Invul,
}PLAYERSTATE;

typedef enum EASEMOVEMENT {
	SpeedUp = 0,
	SlowDown = 1,
	Default = 2,
}EASEMOVEMENT;

typedef struct Player {
	Vector2d startPosition;
	Vector2d endPosition;
	int direction;
	int health;
	sprite playerSprite;
	PLAYERSTATE state;
}Player;

// Initialise player variables
extern void Player_Init();

// Moves the player according to input (WITH DASH MECHANISM)
extern void Player_Update();

// Renders the player location
extern void Player_Render();

// Checks if player's life is 0
extern void Player_CheckGameOver();

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

// Returns health of player
extern int Player_GetHealth();

// Returns sprite of player
extern Player *Player_GetPlayer();

// Returns the attack state of player
extern PLAYERSTATE Player_GetState();

// If player hits the boss, extend his dash
extern void Player_ExtendDash();