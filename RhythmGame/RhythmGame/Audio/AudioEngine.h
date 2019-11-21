#pragma once

#define SILENCE_FACTOR 0.8

typedef enum TRACKTYPE
{
	KICK,
	SNARE,
	BGM,
	SFX,
	PROJECTILE,
	LASER,
	WARNING,
	CLUSTER,
} TRACKTYPE;

typedef enum STAGE {
	MAINMENU,
	TUTORIAL,
	STAGEONE
} STAGE;

/* Call these functions only *************************************************/

// Initialise FMOD for use
extern void Audio_Init();

// Creates a sound object for streaming (provide its path)
extern void Audio_Load(const char *path, TRACKTYPE type);

// Play and forget a SFX (set its volume 0-1)
extern void Audio_PlayOneShot(int id, float volume);

// Play a BGM (provide its magic number/delay)
extern void Audio_PlayBGMWithDelay(double delay, STAGE stage);

// Fade out the BGM
extern void Audio_FadeOutBGM(double time);

// Need to update fmod to get info about state of our fmod variables
extern void Audio_Update();

// Return true if frequency rises above certain threshold
extern double Audio_GetFrequency(TRACKTYPE type);

// Specific which track and set its channel volume
extern void Audio_SetBGMVolume(float volume, TRACKTYPE type);

// Properly unload sounds and close channels
extern void Audio_Shutdown();