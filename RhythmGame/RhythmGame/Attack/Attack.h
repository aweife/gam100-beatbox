#pragma once

#include "../Global.h"
#include "../Text/TextReader.h"
#include "../Enemy/Enemy.h"
#include <stdbool.h>

#define NUMBER_OF_PROJECTILE 200
#define NUMBER_OF_PLAYER_PROJECTILE 30
#define LENGTH_OF_LASER 200
#define NUMBER_OF_LASER 4
#define NUMBER_OF_NOTES 6
#define TYPES_OF_NOTES 3

// To decide which attack to spawn
typedef enum ATTACKTYPE {
	PROJECTILE,
	LASER,
	PLAYER,
}ATTACKTYPE;

// Different speed for player and enemy
typedef struct projectileSpeed {
	double normal;
	double fast;
}projectileSpeed;

// Projectile Struct
typedef struct Projectile {
	Vector2d position;
	bool active;
	DIRECTION direction;
	sprite projectileSprite;
	projectileSpeed speed;
	int which;
} Projectile;

// Laser Struct
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

// Powerup struct
typedef struct Note {
	Vector2d startPosition;
	Vector2d endPosition;
	bool active;
	sprite noteSprite;
	SCORESTATE noteType;
} Note;

// Call this to initialise attacking
extern void Attack_Init();

// Update movement and collision
extern void Attack_Update();

// Render all attacks if they are active
extern void Attack_Render();

// Call this to spawn an attack (provide type and details)
extern void Attack_Spawn(ATTACKTYPE type, Vector2d spawnPosition, DIRECTION direction, projectileSpeed speed, int which);

// Call this to spawn an powerup
extern void Attack_SpawnNote(Vector2d spawnPosition, SCORESTATE type);

// Call this to free sprites used
extern void Attack_Cleanup();
