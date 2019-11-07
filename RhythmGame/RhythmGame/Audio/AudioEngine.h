#pragma once

#include "../Functions.h"
#include "fmod.h"
#include "fmod_errors.h"
#include <stdio.h>

typedef enum
{
	BGM,
	SFX
} TRACK;

////sound control
//extern void setVolume(float v); //sets the actual playing sound's volume
//extern void load(const char *filename); //loads a soundfile
//extern void unload(void); //frees the sound object
//extern void play(int pause); //plays a sound (may be started paused; no argument for unpaused)
//
////getters
//extern int getSound(void); //checks whether the sound is on
//
////setters
//extern void setPause(int pause); //pause or unpause the sound
//extern void setSound(int sound); //set the sound on or off
//
////toggles
//extern void toggleSound(void); //toggles sound on and off
//extern void togglePause(void); //toggle pause on/off

/* Call these functions only *************************************************/

// Initialise FMOD for use
extern void AE_Init();

// Creates a sound object for streaming (provide its path)
extern void AE_LoadTrack(const char *path, TRACK type);

// Play and forget a SFX (set its volume 0-1)
extern void AE_PlayOneShot(int id, float volume);

// Play a BGM (provide its magic number/delay)
extern void AE_StartBGMWithDelay(int id, double delay);

extern void AE_Update();

extern double AE_GetEnergy();

//extern void AE_Play(int trackID, TRACK type);
//extern void AE_SetVolume(float volume, TRACK type);

// For internal use
void _CheckResult(const char *debug);
void _CountDownBGM();