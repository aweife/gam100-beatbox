#include "Enemy.h"
#include "../Attack/Attack.h"
#include "../Clock/Clock.h"
#include "../Global.h"
#include "../Map/Map.h"
#include "../Console/Console.h"
#include "../Audio/AudioEngine.h"
#include "../Random/Random.h"
#include "../UI/GameUI.h"
#include <math.h>

#define ENEMY_BASE_MOVESPEED 0.01
#define ENEMY_FAST_MOVESPEED 0.05
#define PROJECTILE_SPAWN_SPEED 100.0
#define PROJECTILE_SPEED 0.04
#define PROJECTILE_SPEED_FAST 0.1
#define LASER_SPAWN_SPEED 200.0

#define HEALTHBAR_WIDTH 25
#define HEALTHBAR_HEIGHT 4
#define HEALTHBAR_OFFSET_X -2
#define HEALTHBAR_OFFSET_Y -4
#define HEALTH_FEEDBACK 50.0
#define SMALL_PROGRESS 25
#define MEDIUM_PROGRESS 15
#define BIG_PROGRESS 10
#define SMALL_SCORE 10
#define MEDIUM_SCORE 50
#define BIG_SCORE 100

// The skull enemy
Enemy skullEnemy = { 0 };

// Attack speed
static double laserSpawnTimer = 0;
static double projectileSpawnTimer = 0;

// Health
static int progress = 0;
static double damagedTimer = 0;

/* Internal functions */
void _MoveToPosition(double velocity);
void _DecideNextPosition(int position);
void _EnemyAttack();
void _RenderHealthBar();

void Enemy_Init()
{
	// Initialise skull enemy
	skullEnemy.startPosition.x = 90;
	skullEnemy.startPosition.y = 40;
	skullEnemy.startPosition.eulerX = 90.0;
	skullEnemy.startPosition.eulerY = 40.0;
	skullEnemy.nextPosition = skullEnemy.startPosition;
	skullEnemy.enemySprite = Text_CreateSprite();
	Text_Init(&skullEnemy.enemySprite, "..//RhythmGame//$Resources//skull.txt");
	Text_Move(&skullEnemy.enemySprite, skullEnemy.startPosition.x, skullEnemy.startPosition.y);

	// Delay before first attack
	projectileSpawnTimer = 3000.0;
	laserSpawnTimer = 3000.0;

	// Score
	progress = 0;
}

void Enemy_Update()
{
	// If the snare(0) hits, increase enemy speed
	skullEnemy.velocity = (Audio_GetSpectrum(0)) ? ENEMY_FAST_MOVESPEED : ENEMY_BASE_MOVESPEED;
	_MoveToPosition(skullEnemy.velocity);

	_EnemyAttack();

	// Damaged recovery
	if (damagedTimer > 0.0)
		damagedTimer -= Clock_GetDeltaTime();
	else if (damagedTimer <= 0.0)
		skullEnemy.state = NORMAL;
}

void Enemy_Render()
{
	//ASCI ENEMY
	switch (skullEnemy.state)
	{
	default:
		Text_Render(&skullEnemy.enemySprite, 0, Map_GetShakeFactor(UP) / 2);
		break;
	case DAMAGED:
		Text_RenderColor(&skullEnemy.enemySprite, WHITE, 0, Map_GetShakeFactor(UP) / 2);
		break;
	}

	// Debug origin point
	Console_SetRenderBuffer_CharColor(skullEnemy.startPosition.x, skullEnemy.startPosition.y, '+', CYAN);
	Console_SetRenderBuffer_CharColor(skullEnemy.endPosition.x, skullEnemy.endPosition.y, '+', CYAN);

	// Render healthbar state
	_RenderHealthBar();
}

Enemy *Enemy_GetEnemy()
{
	return &skullEnemy;
}

void Enemy_Damage()
{
	if (skullEnemy.state == DAMAGED) return;

	switch (skullEnemy.scoreState)
	{
	case SMALL:
		progress += SMALL_PROGRESS;
		GameUI_AddScore(SMALL_SCORE);
		break;
	case MEDIUM:
		progress += MEDIUM_PROGRESS;
		GameUI_AddScore(MEDIUM_SCORE);
		break;
	case BIG:
		progress += BIG_PROGRESS;
		GameUI_AddScore(MEDIUM_SCORE);
		break;
	}

	skullEnemy.state = DAMAGED;
	damagedTimer = HEALTH_FEEDBACK;
}

void _MoveToPosition(double velocity)
{
	// If reach next position
	if (skullEnemy.startPosition.x == skullEnemy.nextPosition.x && skullEnemy.startPosition.y == skullEnemy.nextPosition.y)
		_DecideNextPosition(Random_Range(1, 6));

	{
		double speed = 1.0 * Clock_GetDeltaTime() * velocity;
		// We move to position with euler
		if (skullEnemy.startPosition.eulerX < skullEnemy.nextPosition.eulerX)
			if ((skullEnemy.startPosition.eulerX + speed) > skullEnemy.nextPosition.eulerX)
				skullEnemy.startPosition.eulerX = skullEnemy.nextPosition.eulerX;
			else
				skullEnemy.startPosition.eulerX += speed;
		else if (skullEnemy.startPosition.eulerX > skullEnemy.nextPosition.eulerX)
			if ((skullEnemy.startPosition.eulerX - speed) < skullEnemy.nextPosition.eulerX)
				skullEnemy.startPosition.eulerX = skullEnemy.nextPosition.eulerX;
			else
				skullEnemy.startPosition.eulerX -= speed;

		if (skullEnemy.startPosition.eulerY < skullEnemy.nextPosition.eulerY)
			if ((skullEnemy.startPosition.eulerY + speed) > skullEnemy.nextPosition.eulerY)
				skullEnemy.startPosition.eulerY = skullEnemy.nextPosition.eulerY;
			else
				skullEnemy.startPosition.eulerY += speed;
		else if (skullEnemy.startPosition.eulerY > skullEnemy.nextPosition.eulerY)
			if ((skullEnemy.startPosition.eulerY - speed) < skullEnemy.nextPosition.eulerY)
				skullEnemy.startPosition.eulerY = skullEnemy.nextPosition.eulerY;
			else
				skullEnemy.startPosition.eulerY -= speed;

		// Update position with euler
		skullEnemy.startPosition.x = (int)skullEnemy.startPosition.eulerX;
		skullEnemy.startPosition.y = (int)skullEnemy.startPosition.eulerY;
		skullEnemy.endPosition.x = (int)(skullEnemy.startPosition.eulerX + skullEnemy.enemySprite.spriteI[skullEnemy.enemySprite.charCount - 1].position.x + 7);
		skullEnemy.endPosition.y = (int)(skullEnemy.startPosition.eulerY + skullEnemy.enemySprite.spriteI[skullEnemy.enemySprite.charCount - 1].position.y);

		Text_Move(&skullEnemy.enemySprite, skullEnemy.startPosition.x, skullEnemy.startPosition.y);
	}
}

void _DecideNextPosition(int position)
{
	switch (position)
	{
	case 1:
		skullEnemy.nextPosition.x = Map_GetOrigin().x + ((Map_GetEnd().x - Map_GetOrigin().x) / 6);
		skullEnemy.nextPosition.y = Map_GetOrigin().y + ((Map_GetEnd().y - Map_GetOrigin().y) / 6);
		break;
	case 2:
		skullEnemy.nextPosition.x = Map_GetOrigin().x + ((Map_GetEnd().x - Map_GetOrigin().x) / 6) * 3;
		skullEnemy.nextPosition.y = Map_GetOrigin().y + ((Map_GetEnd().y - Map_GetOrigin().y) / 6);
		break;
	case 3:
		skullEnemy.nextPosition.x = Map_GetOrigin().x + ((Map_GetEnd().x - Map_GetOrigin().x) / 6) * 5;
		skullEnemy.nextPosition.y = Map_GetOrigin().y + ((Map_GetEnd().y - Map_GetOrigin().y) / 6);
		break;
	case 4:
		skullEnemy.nextPosition.x = Map_GetOrigin().x + ((Map_GetEnd().x - Map_GetOrigin().x) / 6) / 6;
		skullEnemy.nextPosition.y = Map_GetOrigin().y + ((Map_GetEnd().y - Map_GetOrigin().y) / 6) * 3;
		break;
	case 5:
		skullEnemy.nextPosition.x = Map_GetOrigin().x + ((Map_GetEnd().x - Map_GetOrigin().x) / 6) * 3;
		skullEnemy.nextPosition.y = Map_GetOrigin().y + ((Map_GetEnd().y - Map_GetOrigin().y) / 6) * 3;
		break;
	case 6:
		skullEnemy.nextPosition.x = Map_GetOrigin().x + ((Map_GetEnd().x - Map_GetOrigin().x) / 6) * 5;
		skullEnemy.nextPosition.y = Map_GetOrigin().y + ((Map_GetEnd().y - Map_GetOrigin().y) / 6) * 3;
		break;
	case 7:
		skullEnemy.nextPosition.x = Map_GetOrigin().x + ((Map_GetEnd().x - Map_GetOrigin().x) / 6) / 6;
		skullEnemy.nextPosition.y = Map_GetOrigin().y + ((Map_GetEnd().y - Map_GetOrigin().y) / 6) * 5;
		break;
	case 8:
		skullEnemy.nextPosition.x = Map_GetOrigin().x + ((Map_GetEnd().x - Map_GetOrigin().x) / 6) * 3;
		skullEnemy.nextPosition.y = Map_GetOrigin().y + ((Map_GetEnd().y - Map_GetOrigin().y) / 6) * 5;
		break;
	default:
		skullEnemy.nextPosition.x = Map_GetOrigin().x + ((Map_GetEnd().x - Map_GetOrigin().x) / 6) * 5;
		skullEnemy.nextPosition.y = Map_GetOrigin().y + ((Map_GetEnd().y - Map_GetOrigin().y) / 6) * 5;
		break;
	}

	skullEnemy.nextPosition.eulerX = skullEnemy.nextPosition.x;
	skullEnemy.nextPosition.eulerY = skullEnemy.nextPosition.y;

}

void _EnemyAttack()
{
	// If projectile beat(1), spawn projectile
	if (projectileSpawnTimer > 0.0)
		projectileSpawnTimer -= Clock_GetDeltaTime();
	if (laserSpawnTimer > 0.0)
		laserSpawnTimer -= Clock_GetDeltaTime();

	if (laserSpawnTimer <= 0.0)
	{
		if (Audio_GetSpectrum(3))
		{
			Attack_Spawn(LASER, skullEnemy.startPosition, Random_Range(5, 8), (projectileSpeed) { 0, 0 });
			laserSpawnTimer = LASER_SPAWN_SPEED;
		}

		if (Audio_GetSpectrum(1))
		{
			projectileSpeed speed = { PROJECTILE_SPEED,PROJECTILE_SPEED_FAST };
			Attack_Spawn(PROJECTILE, skullEnemy.startPosition, Random_Range(1, 8), speed);
			projectileSpawnTimer = PROJECTILE_SPAWN_SPEED;
		}
	}
}

void _RenderHealthBar()
{
	// Render the background
	for (int i = 0; i < HEALTHBAR_WIDTH; i++)
		for (int j = 0; j < HEALTHBAR_HEIGHT; j++)
			Console_SetRenderBuffer_CharColor(skullEnemy.startPosition.x + HEALTHBAR_OFFSET_X + i, skullEnemy.startPosition.y + HEALTHBAR_OFFSET_Y - j, ' ', GRAY);
}
