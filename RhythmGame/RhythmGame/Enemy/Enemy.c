#include "Enemy.h"
#include "../Attack/Attack.h"
#include "../Clock/Clock.h"
#include "../Global.h"
#include "../Map/Map.h"
#include "../Console/Console.h"
#include "../Audio/AudioEngine.h"
#include "../Random/Random.h"
#include <math.h>

#define ENEMY_BASE_MOVESPEED 0.01
#define ENEMY_FAST_MOVESPEED 0.05
#define PROJECTILE_SPAWN_SPEED 100.0
#define LASER_SPAWN_SPEED 2000.0

// The skull enemy
Enemy skullEnemy = { 0 };

// Attack speed
static double projectileSpawnTimer = 0;

/* Internal functions */
void _MoveToPosition(double velocity);
void _DecideNextPosition(int position);
void _SpawnAttack(ATTACKTYPE type, DIRECTION dir);

void Enemy_Init()
{
	// Initialise skull enemy
	skullEnemy.position.x = 90;
	skullEnemy.position.y = 40;
	skullEnemy.position.eulerX = 90.0;
	skullEnemy.position.eulerY = 40.0;
	skullEnemy.nextPosition = skullEnemy.position;
	skullEnemy.enemySprite = Text_CreateSprite();

	// Delay before first attack
	projectileSpawnTimer = 5000.0;

	Text_Init(&skullEnemy.enemySprite, "..//RhythmGame//$Resources//skull.txt");
	Text_Move(&skullEnemy.enemySprite, skullEnemy.position.x, skullEnemy.position.y);
}

void Enemy_Update()
{
	// If the snare(0) hits, increase enemy speed
	skullEnemy.velocity = (Audio_GetSpectrum(0)) ? ENEMY_FAST_MOVESPEED : ENEMY_BASE_MOVESPEED;
	_MoveToPosition(skullEnemy.velocity);
	Text_Move(&skullEnemy.enemySprite, skullEnemy.position.x, skullEnemy.position.y);

	// If projectile beat(1), spawn projectile
	if (projectileSpawnTimer > 0.0)
		projectileSpawnTimer -= Clock_GetDeltaTime();

	if (projectileSpawnTimer <= 0.0)
	{
		if (Audio_GetSpectrum(2))
		{
			Attack_Spawn(LASER, skullEnemy.position, Random_Range(5, 8));
			projectileSpawnTimer = LASER_SPAWN_SPEED;
		}
		/*if (Audio_GetSpectrum(1))
		{
			Attack_Spawn(PROJECTILE, skullEnemy.position, Random_Range(1, 8));
			projectileSpawnTimer = PROJECTILE_SPAWN_SPEED;
		}*/
	}
}

void Enemy_Render()
{
	//ASCI ENEMY
	Text_Render(&skullEnemy.enemySprite, 0, Map_GetShakeFactor(UP) / 2);

	// Debug origin point
	Console_SetRenderBuffer_CharColor(skullEnemy.position.x, skullEnemy.position.y, '+', CYAN);
}

Enemy *Enemy_GetEnemy()
{
	return &skullEnemy;
}

void _MoveToPosition(double velocity)
{
	// If reach next position
	if (skullEnemy.position.x == skullEnemy.nextPosition.x && skullEnemy.position.y == skullEnemy.nextPosition.y)
		_DecideNextPosition(Random_Range(0, 9));

	{
		double speed = 1.0 * Clock_GetDeltaTime() * velocity;
		// We move to position with euler
		if (skullEnemy.position.eulerX < skullEnemy.nextPosition.eulerX)
			if ((skullEnemy.position.eulerX + speed) > skullEnemy.nextPosition.eulerX)
				skullEnemy.position.eulerX = skullEnemy.nextPosition.eulerX;
			else
				skullEnemy.position.eulerX += speed;
		else if (skullEnemy.position.eulerX > skullEnemy.nextPosition.eulerX)
			if ((skullEnemy.position.eulerX - speed) < skullEnemy.nextPosition.eulerX)
				skullEnemy.position.eulerX = skullEnemy.nextPosition.eulerX;
			else
				skullEnemy.position.eulerX -= speed;

		if (skullEnemy.position.eulerY < skullEnemy.nextPosition.eulerY)
			if ((skullEnemy.position.eulerY + speed) > skullEnemy.nextPosition.eulerY)
				skullEnemy.position.eulerY = skullEnemy.nextPosition.eulerY;
			else
				skullEnemy.position.eulerY += speed;
		else if (skullEnemy.position.eulerY > skullEnemy.nextPosition.eulerY)
			if ((skullEnemy.position.eulerY - speed) < skullEnemy.nextPosition.eulerY)
				skullEnemy.position.eulerY = skullEnemy.nextPosition.eulerY;
			else
				skullEnemy.position.eulerY -= speed;

		// Update position with euler
		skullEnemy.position.x = (int)skullEnemy.position.eulerX;
		skullEnemy.position.y = (int)skullEnemy.position.eulerY;
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

void _SpawnAttack(ATTACKTYPE type, DIRECTION dir)
{
	Attack_Spawn(type, skullEnemy.position, dir);
}