#pragma once

#include "../Functions.h"
#include "fmod.h"
#include "fmod_errors.h"
#include <stdio.h>

typedef enum TRACKTYPE
{
	KICK,
	SNARE,
	BGM,
	SFX,
} TRACKTYPE;

/* Call these functions only *************************************************/

// Initialise FMOD for use
extern void AE_Init();

// Creates a sound object for streaming (provide its path)
extern void AE_LoadTrack(const char *path, TRACKTYPE type);

// Play and forget a SFX (set its volume 0-1)
extern void AE_PlayOneShot(int id, float volume);

// Play a BGM (provide its magic number/delay)
extern void AE_StartBGMWithDelay(int id, double delay);

// Need to update fmod to get info about state of our fmod variables
extern void AE_Update();

// Return true if frequency rises above certain threshold
extern int AE_GetFrequency(TRACKTYPE type);