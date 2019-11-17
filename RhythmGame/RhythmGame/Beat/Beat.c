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

static BeatTimer kick;
static BeatTimer snare;

/* Internal functions */

void _CheckBeats();
void _DecrementTimers();
void _SetBPM(int bpm);

void Beat_Init()
{
	_SetBPM(132);
	snare = (BeatTimer){ .timer = 0,.updated = false, };
	kick = (BeatTimer){ .timer = 0,.updated = false, };
}

void Beat_Update()
{
	_CheckBeats();
	_DecrementTimers();
}

bool Beat_GetKick()
{
	return kick.updated;
}

bool Beat_GetSnare()
{
	return snare.updated;
}

void _CheckBeats()
{
	if (Audio_GetFrequency(KICK) > BEAT_THRESHOLD)
	{
		if (kick.timer <= 0.0)
		{
			kick.updated = !kick.updated;
			kick.timer = bpm;
		}
	}

	if (Audio_GetFrequency(SNARE) > BEAT_THRESHOLD)
	{
		if (snare.timer <= 0.0)
		{
			snare.updated = !snare.updated;
			snare.timer = bpm;
			Enemy_FixedUpdate();
			Attack_FixedUpdate();
		}
	}
}

void _DecrementTimers()
{
	if (snare.timer > 0.0)
		snare.timer -= Clock_GetDeltaTime();

	if (kick.timer > 0.0)
		kick.timer -= Clock_GetDeltaTime();
}

void _SetBPM(int x)
{
	bpm = 60.0 / (double)x;
	bpm *= 800.0;
}