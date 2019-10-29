#include "AudioEngine.h"

static int on; //is sound on?
static int possible; //is it possible to play sound?

static char *currentSound;
static FMOD_SOUND *sound;

#define NUMBER_OF_BGM_TRACKS 1
#define NUMBER_OF_SFX_TRACKS 1
#define MAX_SFX_CHANNELS 10

// State
static bool bIsPlaying;
static bool bHasError;

// FMOD-specific stuff
static FMOD_RESULT result;
static FMOD_SYSTEM *system;

static int bgmTrackCount;
static int sfxTrackCount;
static FMOD_SOUND *bgmSounds[NUMBER_OF_BGM_TRACKS];
static FMOD_SOUND *sfxSounds[NUMBER_OF_SFX_TRACKS];
static FMOD_CHANNEL *bgmChannel;
static FMOD_CHANNEL *sfxChannels[MAX_SFX_CHANNELS];


void AE_Init()
{
	// Initialise state
	bIsPlaying = true;
	bHasError = false;
	bgmTrackCount = 0;
	sfxTrackCount = 0;

	// Setup the sound system
	result = FMOD_System_Create(&system);
	_CheckResult("create");
	if (!bHasError) result = FMOD_System_Init(system, MAX_SFX_CHANNELS + 1, FMOD_INIT_NORMAL, 0);
	_CheckResult("initialising");

	// Set the initial volume for bgm
	if (!bHasError) 
		FMOD_Channel_SetVolume(bgmChannel, 0.1f);
	// Set the initial volume for sfx channels
	if (!bHasError)
		for (int i = 0; i < MAX_SFX_CHANNELS; i++)
			FMOD_Channel_SetVolume(sfxChannels[i], 1.0f);
}

void AE_LoadBGM(const char *path)
{
	_LoadSound(path, &bgmSounds[bgmTrackCount]);
	bgmTrackCount++;
}

void AE_LoadSFX(const char *path)
{
	_LoadSound(path, &sfxSounds[sfxTrackCount]);
	sfxTrackCount++;
}

void AE_PlayBGM(int trackID)
{
	_PlaySound(bgmSounds[trackID], bgmChannel);
}

void AE_PlaySFX(int trackID)
{
	for (int i = 0; i < MAX_SFX_CHANNELS; i++)
	{
		FMOD_BOOL bIsPlaying = 0;
		FMOD_Channel_IsPlaying(sfxChannels[i], &bIsPlaying);
		if (bIsPlaying) continue;

		_PlaySound(sfxSounds[trackID], sfxChannels[i]);
		break;
	}
}

void _CheckResult(const char *debug)
{
	if (result == FMOD_OK) return;

	bHasError = true;
	printf("FMOD ERROR during %s! (%d) %s\n", debug, result, FMOD_ErrorString(result));
}

// Creates a sound object for streaming (provided with its path)
void _LoadSound(const char *path, FMOD_SOUND **sound)
{
	if (!bIsPlaying || bHasError) return;

	result = FMOD_System_CreateStream(system, path, FMOD_CREATESTREAM, 0, sound);
	_CheckResult("loading sound");
}

void _PlaySound(FMOD_SOUND **sound, FMOD_CHANNEL **channel)
{
	if (!bIsPlaying || bHasError) return;

	result = FMOD_System_PlaySound(system, sound, NULL, false, channel);
	_CheckResult("playing");
}



//sets the actual playing sound's volume
void setVolume(float v) {
	if (possible && on && v >= 0.0f && v <= 1.0f) {
		FMOD_Channel_SetVolume(bgmChannel, v);
	}
}

//loads a soundfile
void load(const char *filename) {
	//currentSound = (char *)filename;
	if (possible && on) {
		//result = FMOD_Sound_Release(sound);
		//result = FMOD_System_CreateStream(system, currentSound, FMOD_CREATESTREAM, 0, &sound);
		if (result != FMOD_OK)
		{
			possible = 0;
			printf("FMOD ERROR! (%d) %s\n", result, FMOD_ErrorString(result));
		}
	}
}

//frees the sound object
void unload(void) {
	if (possible) {
		//	result = FMOD_Sound_Release(sound);
	}
}

//plays a sound (no argument to leave pause as dafault)
void play(int pause)
{
	if (possible && on) {
		//	result = FMOD_System_PlaySound(system, sound, NULL, pause, &bgmChannel);
		FMOD_Channel_SetMode(bgmChannel, FMOD_LOOP_NORMAL);
	}
}

//toggles sound on and off
void toggleSound(void)
{
	on = !on;
	if (on == 1) { load(currentSound); play(0); }
	if (on == 0) { unload(); }
}

//pause or unpause the sound
void setPause(int pause)
{
	FMOD_Channel_SetPaused(bgmChannel, pause);
}

//turn sound on or off
void setSound(int s)
{
	on = s;
}

//toggle pause on and off
void togglePause(void)
{
	FMOD_BOOL p;
	FMOD_Channel_GetPaused(bgmChannel, &p);
	FMOD_Channel_SetPaused(bgmChannel, !p);
}

//tells whether the sound is on or off
int getSound(void)
{
	return on;
}