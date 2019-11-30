#pragma once

#include "../Global.h"
#include "../Text/TextReader.h"
#include <stdbool.h>
#include "../States/Game.h"

typedef enum EASEMOVEMENT {
	SpeedUp = 0,
	SlowDown = 1,
	Default = 2,
}EASEMOVEMENT;

typedef enum PLAYERSTATE {
	Normal,
	Invul,
	Damaged,
}PLAYERSTATE;

typedef struct Player {
	Vector2d startPosition; // collision
	Vector2d endPosition; 

	sprite playerSprite; // visual
	PLAYERSTATE state;

	double dashCooldownTimer; // Cooldown of Dash
	double speedUpTimer; // Duration of Dash
	double invulTimer; // Invulnerable timer
	double invulCooldownTimer; // Cooldown of invulnerability

	int direction; // movement
	double velocity; 
	bool collided;

	double factor; // to smooth movement
	double EaseTimer;
	bool EaseCheck;
	bool EaseBool;
}Player;

// Initialise player variables
extern void Player_Init(GAMETYPE type);

// Moves the player according to input (WITH DASH MECHANISM)
extern void Player_Update(int which);

// Renders the player location
extern void Player_Render(int which);

// Allows player to dash
extern void Player_Dash(int which);

// Damages Player
extern void Player_Damage(int which);

// Sets direction and EaseMovement to allow movement in direction with Ease In Ease Out effect
extern void Player_SetVel(DIRECTION dir, EASEMOVEMENT EaseC, int which);

// Returns value of factor
extern double Player_GetEaseFactor(int which);

// Returns direction of player
extern int Player_GetDirection(int which);

// Returns sprite of player
extern Player *Player_GetPlayer(int which);

// Returns the attack state of player
extern PLAYERSTATE Player_GetState(int which);