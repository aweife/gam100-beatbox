#pragma once

#include "../Global.h"
#include "../Text/TextReader.h"
#include "../Enemy/Enemy.h"
#include <stdbool.h>

#define NUMBER_OF_PROJECTILE 100
#define NUMBER_OF_PLAYER_PROJECTILE 20
#define LENGTH_OF_LASER 200
#define NUMBER_OF_LASER 4
#define NUMBER_OF_NOTES 6
#define TYPES_OF_NOTES 3

typedef enum ATTACKTYPE {
	PROJECTILE,
	LASER,
	PLAYER,
}ATTACKTYPE;

typedef struct projectileSpeed {
	double normal;
	double fast;
}projectileSpeed;

//Projectile Structure
typedef struct Projectile {
	Vector2d position;
	bool active;
	DIRECTION direction;
	sprite projectileSprite;
	projectileSpeed speed;
	int which;
} Projectile;

typedef struct Laser {
	Vector2d startPosition;
	Vector2d startPositionCheck;
	Vector2d endPosition;
	Vector2d endPositionCheck;
	DIRECTION direction;
	bool spawned;
	bool active;
	sprite spawnSprite;
	sprite laserSprite[LENGTH_OF_LASER][2];
	int laserIndex;
} Laser;

typedef struct Note {
	Vector2d startPosition;
	Vector2d endPosition;
	bool active;
	sprite noteSprite;
	SCORESTATE noteType;
} Note;

extern void Attack_Init();
extern void Attack_Update();
extern void Attack_Render();
extern void Attack_Spawn(ATTACKTYPE type, Vector2d spawnPosition, DIRECTION direction, projectileSpeed speed, int which);
extern void Attack_SpawnNote(Vector2d spawnPosition, SCORESTATE type);
extern void Attack_Cleanup();
