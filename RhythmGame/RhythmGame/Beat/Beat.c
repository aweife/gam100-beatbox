#include "Beat.h"
#include <stdio.h>
#include "../Clock/Clock.h"
#include "../Map/Map.h"
#include "../Enemy/Enemy.h"
#include "../Player/Player.h"
#include "../Attack/Attack.h"

#define BEAT_THRESHOLD 0.00005121

typedef struct BeatTimer
{
	double timer;
	bool updated;
} BeatTimer;

static double bpm;

static BeatTimer kickTimer;
static BeatTimer snareTimer;
static BeatTimer projectileTimer;
static int projectileDirection = 0;
static BeatTimer warningTimer;
static BeatTimer laserTimer;
static BeatTimer bgmTimer;

/* Internal functions */

void _CheckBeats();
void _DecrementTimers();
void _SetBPM(int bpm);

void Beat_Init()
{
	_SetBPM(650);
	snareTimer = (BeatTimer){ .timer = 0,.updated = false, };
	kickTimer = (BeatTimer){ .timer = 0,.updated = false, };
	projectileTimer = (BeatTimer){ .timer = 0,.updated = false, };
	warningTimer = (BeatTimer){ .timer = 0,.updated = false, };
	laserTimer = (BeatTimer){ .timer = 0,.updated = false, };
	bgmTimer = (BeatTimer){ .timer = 0, .updated = false, };
}

void Beat_Update()
{
	_CheckBeats();
	_DecrementTimers();
}

bool Beat_GetBeat(TRACKTYPE type)
{
	switch (type)
	{
	case KICK:
		return kickTimer.updated;
	case SNARE:
		return snareTimer.updated;
	case PROJECTILE:
		return projectileTimer.updated;
	case WARNING:
		return warningTimer.updated;
	case LASER:
		return laserTimer.updated;
	case BGM:
		return bgmTimer.updated;
	}
}

void _CheckBeats()
{
	if (Audio_GetFrequency(KICK) > BEAT_THRESHOLD)
	{
		if (kickTimer.timer <= 0.0)
		{
			kickTimer.updated = !kickTimer.updated;
			kickTimer.timer = bpm;
		}
	}

	if (Audio_GetFrequency(SNARE) > BEAT_THRESHOLD)
	{
		if (snareTimer.timer <= 0.0)
		{
			snareTimer.updated = !snareTimer.updated;
			snareTimer.timer = bpm;
			Enemy_FixedUpdate();
		}
	}

	if (Audio_GetFrequency(PROJECTILE) > BEAT_THRESHOLD)
	{
		if (projectileTimer.timer <= 0.0)
		{
			projectileTimer.updated = !projectileTimer.updated;
			projectileTimer.timer = bpm;

			// Choose direction to shoot
			DIRECTION dir;
			projectileDirection++;
			if (projectileDirection >= 4) projectileDirection = 0;

			// Calculate distance to travel
			Vector2d currentPosition = Enemy_GetEnemy()->position;
			int distanceToTravel = 0;

			switch (projectileDirection)
			{
			case 0:
				dir = UP;
				//distanceToTravel = currentPosition.y - Map_GetOrigin().y;
				break;
			case 1:
				dir = RIGHT;
				//distanceToTravel = Map_GetEnd().x - currentPosition.x;
				break;
			//case 2:
				//dir = DOWN;
				//distanceToTravel = Map_GetEnd().y - currentPosition.y;
				//	break;
			default:
				dir = LEFT;
				//distanceToTravel = currentPosition.x - Map_GetOrigin().x;
				break;
			}
			
			Attack_SpawnProjectile(currentPosition, dir, 5, 300);
		}
	}

	if (Audio_GetFrequency(WARNING) > BEAT_THRESHOLD)
	{
		if (warningTimer.timer <= 0.0)
		{
			warningTimer.updated = !warningTimer.updated;
			warningTimer.timer = bpm;
			Attack_SpawnLaser(Enemy_GetEnemy()->position, DOWN, 200);
		}
	}

	if (Audio_GetFrequency(LASER) > BEAT_THRESHOLD)
	{
		if (laserTimer.timer <= 0.0)
		{
			laserTimer.updated = !laserTimer.updated;
			laserTimer.timer = bpm;
			Attack_SpawnLaser(Enemy_GetEnemy()->position, DOWN, 200);
		}
	}

	if (Audio_GetFrequency(BGM) > BEAT_THRESHOLD)
	{
		if (bgmTimer.timer <= 0.0)
		{
			bgmTimer.updated = !bgmTimer.updated;
			bgmTimer.timer = bpm;
		}
	}
}

void _DecrementTimers()
{
	if (snareTimer.timer > 0.0)
		snareTimer.timer -= Clock_GetDeltaTime();
	if (kickTimer.timer > 0.0)
		kickTimer.timer -= Clock_GetDeltaTime();
	if (projectileTimer.timer > 0.0)
		projectileTimer.timer -= Clock_GetDeltaTime();
	if (warningTimer.timer > 0.0)
		warningTimer.timer -= Clock_GetDeltaTime();
	if (laserTimer.timer > 0.0)
		laserTimer.timer -= Clock_GetDeltaTime();
	if (bgmTimer.timer > 0.0)
		bgmTimer.timer -= Clock_GetDeltaTime();
}

void _SetBPM(int x)
{
	bpm = 60.0 / (double)x;
	bpm *= 1000.0;
}