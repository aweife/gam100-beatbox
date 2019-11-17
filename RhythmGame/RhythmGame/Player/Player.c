#include "Player.h"
#include "../Global.h"
#include "../Console/Console.h"
#include "../Map/Map.h"
#include "../Enemy/Enemy.h"
#include "../States/StateMachine.h"
#include "../Clock/Clock.h"

Player player;
CONSOLECOLOR color;
static double factor;
static double velocity;

// CHECKS
static int EaseCheck;
static int EaseBool;
static int invulCheck; // Check invulnerable state (1 = invul / 0 = not invul)

// TIMERS
static double dt; // Euler Movement
static double EaseTimer; // Ease
static double cdTimer; // Cooldown of Dash
static double dashTimer; // Duration of Dash
static double invulTimer; // Invulnerable timer

/* Internal functions */

void _MovePlayer();
void _CheckCollision();
// Checks if player is out of border
void _CheckBorder();
// Prints BOXSIZE of player
void _UpdateShape();
// Updates Timer
void _UpdateTimer();


void Player_Init()
{
	player = (Player){
		.direction = STAY,
		.position.x = 50, .position.y = 50,
		.position.eulerX = 50.0, .position.eulerY = 50.0,
		.health = 10,
		.PlayerSprite = Text_CreateSprite(),
		.PlayerSprite.printColor = bRED, };

	Text_Init(&player.PlayerSprite, "..//RhythmGame//$Resources//skull.txt");
	
	factor = 0.0;
	EaseBool = false;
	EaseCheck = SlowDown;
	EaseCheck = true;
	velocity = 0.04;

	// Timers
	EaseTimer = 0.0; // Ease
	cdTimer = 0.0;
	dashTimer = 0.0;
	invulTimer = 0.0;

	// Timer Check
	invulCheck = 0;
}

void Player_Update()
{
	_MovePlayer();
	_CheckBorder();
	_UpdateShape();

	// Check collision after player's position update first
	_CheckCollision();
	_UpdateTimer();
}

void Player_Render()
{
	if (invulCheck == 1)
		color = bBLUE;
	else
		color = bRED;

	for (int i = 0; i < SPRITE_SIZE; i++)
		player.PlayerSprite.printColor[i] = color;

	Text_Render(&player.PlayerSprite);
	

	/*for (int i = 0; i < BOXSIZE * BOXSIZE; i++)
		Console_SetRenderBuffer_CharColor(player.body[i].x, player.body[i].y, ' ', color);*/
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
	if (cdTimer > 0) return;
	velocity = 0.15;
	factor = 1;
	dashTimer = 100.0f;
	cdTimer = 1000.0f;
}

void Player_Damage()
{
	if (invulCheck == 0)
	{
		player.health--;
		invulCheck = 1;
		invulTimer = 2000.0;
	}
}

void _UpdateTimer()
{
	if (invulCheck == 1)
	{
		invulTimer -= Clock_GetDeltaTime();
		
		if (invulTimer < 0)
			invulCheck = 0;
	}
		
}
void _UpdateShape()
{
	int localx = 0;
	int localy = 0;

	localx = player.position.x--;
	localy = player.position.y--;

	for (int i = 0; i < BOXSIZE; i++)
	{
		for (int j = 0; j < BOXSIZE; j++)
		{
			player.body[i * 3 + j].x = localx + j;
			player.body[i * 3 + j].y = localy + i;
		}
	}
}

void _CheckBorder()
{
	if (player.position.eulerX < (MAP_OFFSET + 1)) player.position.eulerX = MAP_OFFSET + 1;
	if (player.position.eulerY< (MAP_OFFSET + 1)) player.position.eulerY = MAP_OFFSET + 1;
	if (player.position.eulerX > (GAME_WIDTH - MAP_OFFSET - BOXSIZE)) player.position.eulerX = GAME_WIDTH - MAP_OFFSET - BOXSIZE;
	if (player.position.eulerY > (GAME_HEIGHT - MAP_OFFSET - BOXSIZE)) player.position.eulerY = GAME_HEIGHT - MAP_OFFSET - BOXSIZE;
}

void _CheckCollision()
{
	// Enemy
	for (int i = 0; i < BOXSIZE * BOXSIZE; i++)
		for (int j = 0; j < SPRITE_SIZE; j++)
			if (player.body[i].x == (Enemy_GetEnemy()->position[j][0] + Enemy_GetEnemy()->Xposition) &&
				player.body[i].y == (Enemy_GetEnemy()->position[j][1] + Enemy_GetEnemy()->Yposition))
				Player_Damage();

	// Projectiles
	for (int i = 0; i < BOXSIZE * BOXSIZE; i++)
		for (int j = 0; j < NUMBER_OF_PROJECTILE; j++)
			if (player.body[i].x == (Enemy_GetProjectile() + j)->position.x &&
				player.body[i].y == (Enemy_GetProjectile() + j)->position.y)
				Player_Damage();
}

void _MovePlayer()
{
	dt = Clock_GetDeltaTime();
	EaseTimer += Clock_GetDeltaTime();

	if (cdTimer > 0) cdTimer -= Clock_GetDeltaTime();
	if (dashTimer > 0) dashTimer -= Clock_GetDeltaTime();
	else velocity = 0.02;

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

	switch (player.direction)
	{
	case TOPLEFT:
		player.position.eulerX += -1 * dt * velocity * factor;
		player.position.eulerY += -1 * dt * velocity * factor;
		break;
	case TOPRIGHT:
		player.position.eulerX += 1 * dt * velocity * factor;
		player.position.eulerY += -1 * dt * velocity * factor;
		break;
	case BOTTOMRIGHT:
		player.position.eulerX += 1 * dt * velocity * factor;
		player.position.eulerY += 1 * dt * velocity * factor;
		break;
	case BOTTOMLEFT:
		player.position.eulerX += -1 * dt * velocity * factor;
		player.position.eulerY += 1 * dt * velocity * factor;
		break;
	case UP: player.position.eulerY += -1 * dt * velocity * factor; break;
	case RIGHT: player.position.eulerX += 1 * dt * velocity * factor; break;
	case DOWN: player.position.eulerY += 1 * dt * velocity * factor; break;
	case LEFT: player.position.eulerX += -1 * dt * velocity * factor; break;
	default: break;
	}

	player.position.x = player.position.eulerX;
	player.position.y = player.position.eulerY;

	Text_Move(&player.PlayerSprite, player.position.x, player.position.y);
}