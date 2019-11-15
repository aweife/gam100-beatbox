#include "Beat.h"
#include <stdio.h>
#include "../Audio/AudioEngine.h"
#include "../Clock/Clock.h"
#include "../Map/Map.h"
#include "../Enemy/Enemy.h"
#include "../Player/Player.h"

#define BEAT_THRESHOLD 0.00015

typedef struct BeatTimer
{
	double timer;
	bool updated;
} BeatTimer;

static double bpm;

static BeatTimer enemy;
static BeatTimer player;

/* Internal functions */

void _CheckBeats();
void _DecrementTimers();
void _SetBPM(int bpm);

void Beat_Init()
{
	_SetBPM(132);
	enemy = (BeatTimer){ .timer = 0,.updated = false, };
	player = (BeatTimer){ .timer = 0,.updated = false, };
}

void Beat_Update()
{
	_CheckBeats();
	_DecrementTimers();
}

void _CheckBeats()
{
	if (Audio_GetFrequency(KICK) > BEAT_THRESHOLD)
	{
		if (enemy.timer <= 0.0)
		{
			enemy.updated = false;

			if (!enemy.updated)
			{
				enemy.updated = true;
				enemy.timer = bpm;
				Enemy_FixedUpdate();
			}
		}
	}

	if (Audio_GetFrequency(SNARE) > BEAT_THRESHOLD)
	{
		if (!player.updated)
		{
			player.updated = true;
			player.timer = 1000.0;
		}
	}
}

void _DecrementTimers()
{
	if (enemy.timer > 0.0)
		enemy.timer -= Clock_GetDeltaTime();

	if (player.timer > 0.0)
		player.timer -= Clock_GetDeltaTime();
}

void _SetBPM(int x)
{
	bpm = 60.0 / (double)x;
	bpm *= 800.0;
}