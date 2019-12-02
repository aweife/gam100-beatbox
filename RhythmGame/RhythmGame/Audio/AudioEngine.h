#pragma once

#include <stdbool.h>

typedef enum STAGE {
	MAINMENU,
	STAGEONE,
	SFX,
} STAGE;

/* Call these functions only *************************************************/

// Initialise FMOD for use
extern void Audio_Create();

// Creates a sound object for streaming (provide its path)
extern void Audio_Load(STAGE stage);

// Unload sound so we dont use too much memory
extern void Audio_Unload();

// Pause the channels or not
extern void Audio_Pause(bool pause);

// Play and forget a SFX (set its volume 0-1)
extern void Audio_PlayOneShot(int id, float volume);

// Play a BGM (provide its magic number/delay)
extern void Audio_PlayBGM(STAGE stage);

// Return true if frequency rises above certain threshold
extern bool Audio_GetSpectrum(int id);

// Need to update fmod to get info about state of our fmod variables
extern void Audio_Update();

// Properly unload sounds and close channels
extern void Audio_Shutdown();