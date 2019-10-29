#pragma once
#include <stdio.h>
#include "fmod.h"
#include "fmod_errors.h"

#define bool int
#define true 1
#define false 0

extern void initialise(void); //initialises sound

//sound control
extern void setVolume(float v); //sets the actual playing sound's volume
extern void load(const char *filename); //loads a soundfile
extern void unload(void); //frees the sound object
extern void play(int pause); //plays a sound (may be started paused; no argument for unpaused)

//getters
extern int getSound(void); //checks whether the sound is on

//setters
extern void setPause(int pause); //pause or unpause the sound
extern void setSound(int sound); //set the sound on or off

//toggles
extern void toggleSound(void); //toggles sound on and off
extern void togglePause(void); //toggle pause on/off

// Use these functions only
extern void AE_Init();
extern void AE_LoadBGM(const char *path);
extern void AE_LoadSFX(const char *path);
extern void AE_PlayBGM(int trackID);
extern void AE_PlaySFX(int trackID);

// For internal use
void _CheckResult(const char *debug);

void _LoadSound(const char *path, FMOD_SOUND **sound);

void _PlaySound(FMOD_SOUND **sound, FMOD_CHANNEL **channel);