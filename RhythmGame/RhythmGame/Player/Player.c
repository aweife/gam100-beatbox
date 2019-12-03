/*******************************************************************************
* @filename Player.c
* @author(s) Wong Swee Jong Nico
* @DP emails s.wong@digipen.edu
* Brief Description:
* This file contains the Player functions.
* COPYRIGHT © 2019 BY DIGIPEN CORP, USA. ALL RIGHTS RESERVED.
*******************************************************************************/

#include "Player.h"
#include "../Console/Console.h"
#include "../Map/Map.h"
#include "../Enemy/Enemy.h"
#include "../States/StateMachine.h"
#include "../Clock/Clock.h"
#include "../UI/GameUI.h"
#include "../Audio/AudioEngine.h"
#include "../Attack/Attack.h"
#include "../States/Game.h"

#define DEBUG_AABB 0
#define SPRITE_OFFSET 1

#define PLAYER_BASE_MOVESPEED 0.06
#define PLAYER_FAST_MOVESPEED 0.3
#define PLAYER_INVUL_DURATION 1500.0
#define PLAYER_INVUL_COOLDOWN 1000.0
#define PLAYER_DASH_COOLDOWN 500.0
#define PLAYER_ATTACKSPEED 250.0
#define PROJECTILE_SPEED 0.1

static Player player[2] = { 0 };
static bool godMode = 0;

// TIMERS
static double attackTimer;

/* Internal functions */

// Logic for player movement depending on user input
void _MovePlayer(int which);

// Checks if player collision with border
void _CheckBorder(int which); 

// Updates timers
void _UpdateState(int which);

// Allows player to shoot automatically 
void _AutoAttack();


void Player_Init(GAMETYPE type)
{
	int numberOfPlayers = 1;
	if (type == TUT)
		godMode = true;
	else
		godMode = false;

	if (type == TWOPLAYER)
		numberOfPlayers = 2;


	// Init one or two players
	for (int i = 0; i < numberOfPlayers; i++)
	{
		player[i].direction = STAY;
		player[i].state = Normal;
		player[i].playerSprite = Text_CreateSprite();
		Text_Init(&player[i].playerSprite, "$Resources//Sprites//Game//player.txt");

		// Movement
		player[i].factor = 0.0;
		player[i].EaseBool = false;
		player[i].EaseCheck = SlowDown;
		player[i].EaseCheck = true;
		player[i].velocity = 0.04;
		player[i].collided = false;

		// Timers
		player[i].EaseTimer = 0.0; // Ease
		player[i].dashCooldownTimer = 0.0;
		player[i].speedUpTimer = 0.0;
		player[i].invulTimer = 0.0;

		// Set their starting positions Y
		player[i].startPosition.eulerY = player[i].startPosition.y = 125;
	}

	// Set their starting positions X
	if (type == TWOPLAYER)
	{
		player[0].startPosition.eulerX = player[0].startPosition.x = 170;
		player[1].startPosition.eulerX = player[1].startPosition.x = 20;
	}
	else
		player[0].startPosition.eulerX = player[0].startPosition.x = 95;

	if (type == TUT)
		player[0].startPosition.eulerY = player[0].startPosition.y = 100;
}

void Player_Update(int which)
{
	// Check against the border
	_CheckBorder(which);
	_AutoAttack();

	_MovePlayer(which);
	_UpdateState(which);
}

void Player_Render(int which)
{
	CONSOLECOLOR c, d;
	switch (player[which].state)
	{
	case Damaged:
		c = RED;
		d = DARKRED;
		break;
	case Invul:
		c = which ? CYAN : MAGENTA;
		d = which ? DARKCYAN : DARKMAGENTA;
		break;
	default:
		c = which ? GREEN : BLUE;
		d = which ? DARKGREEN : DARKBLUE;
		break;
	}
	for (int i = 0, count = player[which].playerSprite.charCount; i < count; i++)
		if (player[which].playerSprite.spriteI[i].printchar == 'b')
			player[which].playerSprite.spriteI[i].printColor = c;
		else if (player[which].playerSprite.spriteI[i].printchar == 'B')
			player[which].playerSprite.spriteI[i].printColor = d;

	Text_Render(&player[which].playerSprite, Map_GetShakeFactor(RIGHT) / 2, 0);

	if (DEBUG_AABB)
	{
		// Debug origin
		Console_SetRenderBuffer_CharColor(player[which].startPosition.x, player[which].startPosition.y, '+', CYAN);
		// Debug endposition
		Console_SetRenderBuffer_CharColor(player[which].endPosition.x, player[which].endPosition.y, '+', CYAN);
	}
}

void Player_SetVel(DIRECTION dir, EASEMOVEMENT EaseC, int which)
{
	player[which].direction = dir;
	player[which].EaseCheck = EaseC;
}

double Player_GetEaseFactor(int which)
{
	return player[which].factor;
}

int Player_GetDirection(int which)
{
	return player[which].direction;
}

void Player_Dash(int which)
{
	if (player[which].state == Damaged) return;
	// If cdTimer is running
	if (player[which].dashCooldownTimer > 0.0)
		return;

	player[which].factor = 1;
	player[which].speedUpTimer = 100.0;
	if (player[which].invulCooldownTimer <= 0.0)
	{
		player[which].state = Invul;
		player[which].invulTimer = PLAYER_INVUL_DURATION;
	}
	player[which].invulCooldownTimer = PLAYER_INVUL_COOLDOWN;
	player[which].dashCooldownTimer = PLAYER_DASH_COOLDOWN;
}

void Player_Damage(int which)
{
	if (player[which].invulTimer > 0.0) return;

	player[which].invulTimer = PLAYER_INVUL_DURATION;
	player[which].state = Damaged;
	Map_Shake(RIGHT, 60.0, MAP_SHAKE_X);
	if (!godMode)
		GameUI_DecreaseHealth(Game_GetGameType() ? 1 : 2, which);
}

Player *Player_GetPlayer(int which)
{
	return &player[which];
}

PLAYERSTATE Player_GetState(int which)
{
	return player[which].state;
}

void _UpdateState(int which)
{
	player[which].EaseTimer += Clock_GetDeltaTime();

	// TIMERS
	if (player[which].invulCooldownTimer > 0.0)
		player[which].invulCooldownTimer -= Clock_GetDeltaTime();
	if (player[which].speedUpTimer > 0.0)
		player[which].speedUpTimer -= Clock_GetDeltaTime();
	if (player[which].dashCooldownTimer > 0.0)
		player[which].dashCooldownTimer -= Clock_GetDeltaTime();
	if (player[which].invulTimer > 0.0)
		player[which].invulTimer -= Clock_GetDeltaTime();
	else if (player[which].invulTimer <= 0.0)
		player[which].state = Normal;

	if (player[which].EaseTimer >= 25.0)
	{
		if (player[which].EaseCheck == SpeedUp)
			player[which].factor += 0.1;
		else if (player[which].EaseCheck == SlowDown)
			player[which].factor -= 0.1;

		if (player[which].factor < 0)
			player[which].factor = 0.0;
		else if (player[which].factor > 1.0)
			player[which].factor = 1.0;
		player[which].EaseTimer -= player[which].EaseTimer;
	}
}

void _CheckBorder(int which)
{
	if (player[which].startPosition.x < Map_GetOrigin().x + 2)
		player[which].startPosition.eulerX = (double)Map_GetOrigin().x + 2;
	if (player[which].startPosition.y < Map_GetOrigin().y + 2)
		player[which].startPosition.eulerY = (double)Map_GetOrigin().y + 2;
	if (player[which].endPosition.x > Map_GetEnd().x - 2)
		player[which].startPosition.eulerX = (double)Map_GetEnd().x - 1 - player[which].playerSprite.spriteI[player[which].playerSprite.charCount - 1].position.x - 1;
	if (player[which].endPosition.y > Map_GetEnd().y - 2)
		player[which].startPosition.eulerY = (double)Map_GetEnd().y - 1 - player[which].playerSprite.spriteI[player[which].playerSprite.charCount - 1].position.y;
}

void _MovePlayer(int which)
{
	if (player[which].collided) return;

	player[which].velocity = (player[which].speedUpTimer > 0.0) ? PLAYER_FAST_MOVESPEED : PLAYER_BASE_MOVESPEED;
	double speed = Clock_GetDeltaTime() * player[which].velocity * player[which].factor;
	switch (player[which].direction)
	{
	case TOPLEFT:
		player[which].startPosition.eulerX -= speed / 1.4;
		player[which].startPosition.eulerY -= speed / 1.4;
		break;
	case TOPRIGHT:
		player[which].startPosition.eulerX += speed / 1.4;
		player[which].startPosition.eulerY -= speed / 1.4;
		break;
	case BOTTOMRIGHT:
		player[which].startPosition.eulerX += speed / 1.4;
		player[which].startPosition.eulerY += speed / 1.4;
		break;
	case BOTTOMLEFT:
		player[which].startPosition.eulerX -= speed / 1.4;
		player[which].startPosition.eulerY += speed / 1.4;
		break;
	case UP:
		player[which].startPosition.eulerY -= speed; break;
	case RIGHT:
		player[which].startPosition.eulerX += speed; break;
	case DOWN:
		player[which].startPosition.eulerY += speed; break;
	case LEFT:
		player[which].startPosition.eulerX -= speed; break;
	default: break;
	}

	player[which].startPosition.x = (int)player[which].startPosition.eulerX;
	player[which].startPosition.y = (int)player[which].startPosition.eulerY;
	player[which].endPosition.x = (int)(player[which].startPosition.eulerX + player[which].playerSprite.spriteI[player[which].playerSprite.charCount - 1].position.x + SPRITE_OFFSET);
	player[which].endPosition.y = (int)(player[which].startPosition.eulerY + player[which].playerSprite.spriteI[player[which].playerSprite.charCount - 1].position.y);
	player[which].endPosition.eulerX = player[which].startPosition.eulerX + player[which].playerSprite.spriteI[player[which].playerSprite.charCount - 1].position.x + SPRITE_OFFSET;
	player[which].endPosition.eulerY = player[which].startPosition.eulerY + player[which].playerSprite.spriteI[player[which].playerSprite.charCount - 1].position.y;

	Text_Move(&player[which].playerSprite, player[which].startPosition.x, player[which].startPosition.y);
}

void _AutoAttack()
{
	if (attackTimer > 0.0)
		attackTimer -= Clock_GetDeltaTime();
	else if (attackTimer <= 0.0)
	{
		attackTimer = PLAYER_ATTACKSPEED;
		projectileSpeed speed = { PROJECTILE_SPEED,PROJECTILE_SPEED };
		Vector2d middle = (Vector2d){
			.x = (player[0].startPosition.x + player[0].endPosition.x) / 2,
			.y = player[0].startPosition.y,
			.eulerX = (player[0].startPosition.eulerX + player[0].endPosition.eulerX) / 2.0,
			.eulerY = player[0].startPosition.eulerY,
		};
		Attack_Spawn(PLAYER, middle, UP, speed, 0);
		if (Game_GetGameType() == TWOPLAYER)
		{
			Vector2d middle2 = (Vector2d){
			.x = (player[1].startPosition.x + player[1].endPosition.x) / 2,
			.y = player[1].startPosition.y,
			.eulerX = (player[1].startPosition.eulerX + player[1].endPosition.eulerX) / 2.0,
			.eulerY = player[1].startPosition.eulerY,
			};
			Attack_Spawn(PLAYER, middle2, UP, speed, 1);
		}
	}
}