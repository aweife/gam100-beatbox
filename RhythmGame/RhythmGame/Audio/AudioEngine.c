#include "AudioEngine.h"

//static int on; //is sound on?
//static int possible; //is it possible to play sound?
//
//static char *currentSound;
//static FMOD_SOUND *sound;

#define NUMBER_OF_BGM_TRACKS 1
#define NUMBER_OF_SFX_TRACKS 1
#define NUMBER_OF_CHANNELS 20

// State
static bool bIsPlaying;
static bool bHasError;

// FMOD-specific stuff
static FMOD_RESULT result;
static FMOD_SYSTEM *fmodSystem;

// BGM
static int bgmCount;
static FMOD_SOUND *bgmSounds[NUMBER_OF_BGM_TRACKS];
static FMOD_CHANNEL* bgmChannel;
static double bgmDelay;

// SFX
static int sfxCount;
static FMOD_SOUND *sfxList[NUMBER_OF_SFX_TRACKS];


void AE_Init()
{
	// Initialise state
	bIsPlaying = true;
	bHasError = false;
	bgmCount = 0;
	sfxCount = 0;

	// Setup the sound system
	result = FMOD_System_Create(&fmodSystem);
	_CheckResult("create");

	// Initialise sound system
	if (!bHasError) result = FMOD_System_Init(fmodSystem, NUMBER_OF_CHANNELS, FMOD_INIT_NORMAL, 0);
	_CheckResult("initialising");
}

void AE_LoadTrack(const char *path, TRACK type)
{
	switch (type)
	{
	case BGM:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_DEFAULT, 0, &bgmSounds[bgmCount]);
		bgmCount++;
		break;
	case SFX:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_DEFAULT, 0, &sfxList[sfxCount]);
		sfxCount++;
		break;
	}
	_CheckResult("loading sound");
}

void AE_PlayOneShot(int id, float volume)
{
	// Set the track paused
	FMOD_CHANNEL *c;
	result = FMOD_System_PlaySound(fmodSystem, sfxList[id], 0, true, &c);
	_CheckResult("playing");

	// Set the volume of the one shot
	if (volume < 0.0f || volume > 1.0f) return;
	result = FMOD_Channel_SetVolume(c, volume);
	_CheckResult("setting volume");

	// Unpause the track
	result = FMOD_Channel_SetPaused(c, false);
	_CheckResult("playing");
}

void AE_StartBGMWithDelay(int id, double delay)
{
	bgmDelay = delay * 1000.0;

	result = FMOD_System_PlaySound(fmodSystem, sfxList[id], 0, true, &bgmChannel);
	_CheckResult("play bgm paused");
}

void AE_Update()
{
	result = FMOD_System_Update(fmodSystem);
	_CheckResult("updating");

	// If StartBGMWithDelay has been called
	if (bgmDelay > 0.0)
		_CountDownBGM();
}

void AE_Shutdown()
{
	// All channels stop playing and released, main system too
	result = FMOD_System_Release(fmodSystem);
	_CheckResult("system shutdown");

	// Free sounds in memory
	for (int i = 0; i < bgmCount; i++)
		FMOD_Sound_Release(bgmSounds[i]);
	_CheckResult("system shutdown");
	for (int i = 0; i < sfxCount; i++)
		FMOD_Sound_Release(sfxList[i]);
	_CheckResult("system shutdown");
}

void _CheckResult(const char *debug)
{
	if (result == FMOD_OK) return;

	bHasError = true;
	printf("FMOD ERROR during %s! (%d) %s\n", debug, result, FMOD_ErrorString(result));
}

void _CountDownBGM()
{
	bgmDelay -= Clock_GetDeltaTime();
	
	// Time the delay
	if (bgmDelay <= 0.0)
	{
		bgmDelay = 0.0;

		result = FMOD_Channel_SetPaused(bgmChannel, false);
		_CheckResult("starting bgm with delay");
	}
}

//void AE_Play(int trackID, TRACK type)
//{
//	switch (type)
//	{
//	case BGM:
//		// Returns the channel handle for manipulation
//		result = FMOD_System_PlaySound(system, bgmSounds[trackID], 0, false, &bgmChannel);
//		_CheckResult("playing");
//		break;
//	case SFX:
//		// Automatically plays oneshot
//		result = FMOD_System_PlaySound(system, sfxList[trackID], 0, false, 0);
//		_CheckResult("playing");
//		break;
//	}
//}

//void AE_SetVolume(float volume, TRACK type)
//{
//	switch (type)
//	{
//	case BGM:
//		result = FMOD_Channel_SetVolume(bgmChannel, volume);
//		break;
//	case SFX:
//		//result = FMOD_ChannelGroup_SetVolume(sfxGroup, volume);
//		break;
//	}
//	_CheckResult("setting volume");
//}

//sets the actual playing sound's volume
//void setVolume(float v) {
//	if (possible && on && v >= 0.0f && v <= 1.0f) {
//		FMOD_Channel_SetVolume(bgmChannel, v);
//	}
//}
//
////loads a soundfile
//void load(const char *filename) {
//	//currentSound = (char *)filename;
//	if (possible && on) {
//		//result = FMOD_Sound_Release(sound);
//		//result = FMOD_System_CreateStream(system, currentSound, FMOD_CREATESTREAM, 0, &sound);
//		if (result != FMOD_OK)
//		{
//			possible = 0;
//			printf("FMOD ERROR! (%d) %s\n", result, FMOD_ErrorString(result));
//		}
//	}
//}
//
////frees the sound object
//void unload(void) {
//	if (possible) {
//		//	result = FMOD_Sound_Release(sound);
//	}
//}
//
////plays a sound (no argument to leave pause as dafault)
//void play(int pause)
//{
//	if (possible && on) {
//		//	result = FMOD_System_PlaySound(system, sound, NULL, pause, &bgmChannel);
//		FMOD_Channel_SetMode(bgmChannel, FMOD_LOOP_NORMAL);
//	}
//}
//
////toggles sound on and off
//void toggleSound(void)
//{
//	on = !on;
//	if (on == 1) { load(currentSound); play(0); }
//	if (on == 0) { unload(); }
//}
//
////pause or unpause the sound
//void setPause(int pause)
//{
//	FMOD_Channel_SetPaused(bgmChannel, pause);
//}
//
////turn sound on or off
//void setSound(int s)
//{
//	on = s;
//}
//
////toggle pause on and off
//void togglePause(void)
//{
//	FMOD_BOOL p;
//	FMOD_Channel_GetPaused(bgmChannel, &p);
//	FMOD_Channel_SetPaused(bgmChannel, !p);
//}
//
////tells whether the sound is on or off
//int getSound(void)
//{
//	return on;
//}