#include "Player.h"
#include "../Console/Console.h"
#include "../Map/Map.h"
#include "../Enemy/Enemy.h"
#include "../States/StateMachine.h"
#include "../Clock/Clock.h"
#include "../UI/GameUI.h"
#include "../Audio/AudioEngine.h"
#include "../Attack/Attack.h"

#define DEBUG_AABB 0
#define COLLISION_OFFSET 1

#define PLAYER_BASE_MOVESPEED 0.06
#define PLAYER_FAST_MOVESPEED 0.3
#define PLAYER_INVUL_DURATION 1000.0
#define PLAYER_INVUL_COOLDOWN 1000.0
#define PLAYER_DASH_COOLDOWN 300.0
#define PLAYER_ATTACKSPEED 300.0
#define PROJECTILE_SPEED 0.1

static Player player;
static double factor;
static double velocity;

// CHECKS
static int EaseCheck;
static int EaseBool;
static bool canAttack;

// TIMERS
static double dt; // Euler Movement
static double EaseTimer; // Ease
static double dashCooldownTimer; // Cooldown of Dash
static double speedUpTimer; // Duration of Dash
static double invulTimer; // Invulnerable timer
static double invulCooldownTimer; // Cooldown of invul
static double attackTimer;

/* Internal functions */

void _MovePlayer();
void _CheckBorder(); // Checks if player collision with border
void _UpdateState();// Updates timers
void _AutoAttack();


void Player_Init()
{
	player.direction = STAY;
	player.startPosition.eulerX = player.startPosition.x = 95;
	player.startPosition.eulerY = player.startPosition.y = 80;
	player.playerSprite = Text_CreateSprite();
	player.state = Normal;

	Text_Init(&player.playerSprite, "..//RhythmGame//$Resources//player.txt");

	factor = 0.0;
	EaseBool = false;
	EaseCheck = SlowDown;
	EaseCheck = true;
	canAttack = true;
	velocity = 0.04;

	// Timers
	EaseTimer = 0.0; // Ease
	dashCooldownTimer = 0.0;
	speedUpTimer = 0.0;
	invulTimer = 0.0;
}

void Player_Update()
{
	_MovePlayer();
	_UpdateState();

	// Check against the border
	_CheckBorder();
	_AutoAttack();
}

void Player_Render()
{
	CONSOLECOLOR c, d;
	switch (player.state)
	{
	case Damaged:
		c = RED;
		d = DARKRED;
		break;
	case Invul:
		c = CYAN;
		d = DARKCYAN;
		break;
	default:
		c = BLUE;
		d = DARKBLUE;
		break;
	}
	for (int i = 0, count = player.playerSprite.charCount; i < count; i++)
		if (player.playerSprite.spriteI[i].printchar == 'b')
			player.playerSprite.spriteI[i].printColor = c;
		else if (player.playerSprite.spriteI[i].printchar == 'B')
			player.playerSprite.spriteI[i].printColor = d;

	Text_Render(&player.playerSprite, Map_GetShakeFactor(RIGHT) / 2, 0);

	if (DEBUG_AABB)
	{
		// Debug origin
		Console_SetRenderBuffer_CharColor(player.startPosition.x, player.startPosition.y, '+', CYAN);
		// Debug endposition
		Console_SetRenderBuffer_CharColor(player.endPosition.x, player.endPosition.y, '+', CYAN);
	}
}

void Player_SetVel(DIRECTION dir, EASEMOVEMENT EaseC)
{
	player.direction = dir;
	EaseCheck = EaseC;
}

double Player_GetEaseFactor()
{
	return factor;
}

int Player_GetDirection()
{
	return player.direction;
}

void Player_Dash()
{
	// If cdTimer is running
	if (dashCooldownTimer > 0.0)
		return;

	factor = 1;
	speedUpTimer = 100.0;
	if (invulCooldownTimer <= 0.0)
	{
		player.state = Invul;
		invulTimer = PLAYER_INVUL_DURATION;
	}
	invulCooldownTimer = PLAYER_INVUL_COOLDOWN;
	dashCooldownTimer = PLAYER_DASH_COOLDOWN;
}

void Player_ExtendDash()
{
	if (!canAttack) return;
	canAttack = false;
	factor = 1;
	speedUpTimer += 20.0;
	player.state = Invul;
	invulTimer += 100.0;
}

void Player_Damage()
{
	if (invulTimer > 0.0) return;

	invulTimer = PLAYER_INVUL_DURATION;
	player.state = Damaged;
	Map_Shake(RIGHT, 80.0, MAP_SHAKE_X);
	GameUI_DecreaseHealth(1);
}

Player *Player_GetPlayer()
{
	return &player;
}

PLAYERSTATE Player_GetState()
{
	return player.state;
}

void _UpdateState()
{
	dt = Clock_GetDeltaTime();
	EaseTimer += Clock_GetDeltaTime();

	if (invulTimer > 0.0)
		invulTimer -= Clock_GetDeltaTime();
	else if (invulTimer <= 0.0)
		player.state = Normal;
	if (invulCooldownTimer > 0.0)
		invulCooldownTimer -= Clock_GetDeltaTime();
	if (speedUpTimer > 0.0)
		speedUpTimer -= Clock_GetDeltaTime();
	if (dashCooldownTimer > 0.0)
		dashCooldownTimer -= Clock_GetDeltaTime();
	else if (dashCooldownTimer <= 0.0)
		canAttack = true;

	if (EaseTimer >= 25.0)
	{
		if (EaseCheck == SpeedUp)
			factor += 0.1;
		else if (EaseCheck == SlowDown)
			factor -= 0.1;

		if (factor < 0)
			factor = 0.0;
		else if (factor > 1.0)
			factor = 1.0;
		EaseTimer -= EaseTimer;
	}
}

void _CheckBorder()
{
	if (player.startPosition.x < Map_GetOrigin().x + 2)
		player.startPosition.eulerX = Map_GetOrigin().x + 2;
	if (player.startPosition.y < Map_GetOrigin().y + 2)
		player.startPosition.eulerY = Map_GetOrigin().y + 2;
	if (player.endPosition.x > Map_GetEnd().x - 2)
		player.startPosition.eulerX = Map_GetEnd().x - 1 - player.playerSprite.spriteI[player.playerSprite.charCount - 1].position.x - 1;
	if (player.endPosition.y > Map_GetEnd().y - 2)
		player.startPosition.eulerY = Map_GetEnd().y - 1 - player.playerSprite.spriteI[player.playerSprite.charCount - 1].position.y;
}

void _MovePlayer()
{
	velocity = (speedUpTimer > 0.0) ? PLAYER_FAST_MOVESPEED : PLAYER_BASE_MOVESPEED;
	double speed = 1.0 * dt * velocity * factor;
	switch (player.direction)
	{
	case TOPLEFT:
		player.startPosition.eulerX -= speed / 1.4;
		player.startPosition.eulerY -= speed / 1.4;
		break;
	case TOPRIGHT:
		player.startPosition.eulerX += speed / 1.4;
		player.startPosition.eulerY -= speed / 1.4;
		break;
	case BOTTOMRIGHT:
		player.startPosition.eulerX += speed / 1.4;
		player.startPosition.eulerY += speed / 1.4;
		break;
	case BOTTOMLEFT:
		player.startPosition.eulerX -= speed / 1.4;
		player.startPosition.eulerY += speed / 1.4;
		break;
	case UP:
		player.startPosition.eulerY -= speed; break;
	case RIGHT:
		player.startPosition.eulerX += speed; break;
	case DOWN:
		player.startPosition.eulerY += speed; break;
	case LEFT:
		player.startPosition.eulerX -= speed; break;
	default: break;
	}

	player.startPosition.x = (int)player.startPosition.eulerX;
	player.startPosition.y = (int)player.startPosition.eulerY;
	player.endPosition.x = (int)(player.startPosition.eulerX + player.playerSprite.spriteI[player.playerSprite.charCount - 1].position.x + COLLISION_OFFSET);
	player.endPosition.y = (int)(player.startPosition.eulerY + player.playerSprite.spriteI[player.playerSprite.charCount - 1].position.y);

	Text_Move(&player.playerSprite, player.startPosition.x, player.startPosition.y);
}

void _AutoAttack()
{
	if (attackTimer > 0.0)
		attackTimer -= Clock_GetDeltaTime();
	else if (attackTimer <= 0.0)
	{
		attackTimer = PLAYER_ATTACKSPEED;
		projectileSpeed speed = { PROJECTILE_SPEED,PROJECTILE_SPEED };
		Attack_Spawn(PLAYER, player.startPosition, UP, speed);
	}
}