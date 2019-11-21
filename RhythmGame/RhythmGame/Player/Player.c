#include "Player.h"
#include "../Global.h"
#include "../Console/Console.h"
#include "../Map/Map.h"
#include "../Enemy/Enemy.h"
#include "../States/StateMachine.h"
#include "../Clock/Clock.h"
#include "../UI/GameUI.h"

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

/* Internal functions */

void _MovePlayer();
// Checks if player is out of border
void _CheckBorder();
// Updates Timer
void _UpdateState();


void Player_Init()
{
	player = (Player){
		.direction = STAY,
		.startPosition.x = 100, .startPosition.y = 100,
		.startPosition.eulerX = 100.0, .startPosition.eulerY = 100.0,
		.health = 10,
		.playerSprite = Text_CreateSprite(),
		.state = Normal,
	};

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
}

void Player_Render()
{
	CONSOLECOLOR c, d;
	switch (player.state)
	{
	case Invul:
		c = RED;
		d = DARKRED;
		break;
	default:
		c = BLUE;
		d = DARKBLUE;
		break;
	}
	for (int i = 0; i < SPRITE_SIZE; i++)
		if(player.playerSprite.printchar[i] == 'b')
			player.playerSprite.printColor[i] = c;
		else if(player.playerSprite.printchar[i] == 'B')
			player.playerSprite.printColor[i] = d;

	Text_Render(&player.playerSprite, Map_GetShakeFactor(RIGHT) / 2, 0);
	// Debug origin
	Console_SetRenderBuffer_CharColor(player.startPosition.x, player.startPosition.y, '+', CYAN);
	// Debug endposition
	Console_SetRenderBuffer_CharColor(player.endPosition.x, player.endPosition.y, '+', CYAN);
}

void Player_SetVel(DIRECTION dir, EASEMOVEMENT EaseC)
{
	player.direction = dir;
	EaseCheck = EaseC;
}

int Player_GetHealth()
{
	return player.health;
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
	invulTimer = 200.0;
	player.state = Dash;
	dashCooldownTimer = 1000.0;
}

void Player_ExtendDash()
{
	if (!canAttack) return;
	canAttack = false;
	factor = 1;
	speedUpTimer += 20.0;
	player.state = ExDash;
	invulTimer += 100.0;
	Map_Shake(UP, 40.0, MAP_SHAKE_Y);
}

void Player_Damage()
{
	if (invulTimer > 0.0) return;

	player.health--;
	player.state = Invul;
	invulTimer = 2000.0;
	Map_Shake(RIGHT, 100.0, MAP_SHAKE_X);
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
	if (speedUpTimer > 0.0)
		speedUpTimer -= Clock_GetDeltaTime();
	if (dashCooldownTimer > 0.0)
		dashCooldownTimer -= Clock_GetDeltaTime();

	switch (player.state)
	{
	case Invul:
		if (invulTimer < 0.0)
			player.state = Normal;
		break;
	case Dash:
		if (speedUpTimer < 0.0)
			player.state = Normal;
		break;
	case ExDash:
		if (speedUpTimer < 0.0)
			player.state = Normal;
		break;
	default:
		if (dashCooldownTimer < 0.0)
			canAttack = true;
		break;
	}

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
		player.startPosition.eulerX = Map_GetEnd().x - 1 - player.playerSprite.position[player.playerSprite.charCount - 1].x-1;
	if (player.endPosition.y > Map_GetEnd().y - 2)
		player.startPosition.eulerY = Map_GetEnd().y - 1 - player.playerSprite.position[player.playerSprite.charCount - 1].y;
}

void _MovePlayer()
{
	switch (player.state)
	{
	case ExDash:
		velocity = 0.35;
		break;
	case Dash:
		velocity = 0.15;
		break;
	default:
		velocity = 0.02;
		break;
	}
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
	player.endPosition.x = (int)(player.startPosition.eulerX + player.playerSprite.position[player.playerSprite.charCount - 1].x+1);
	player.endPosition.y = (int)(player.startPosition.eulerY + player.playerSprite.position[player.playerSprite.charCount - 1].y);

	Text_Move(&player.playerSprite, player.startPosition.x, player.startPosition.y);
}