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
typedef struct track
{
	FMOD_DSP *dsp;
	FMOD_DSP_PARAMETER_FFT *dspFFT;
	FMOD_SOUND *sound;
	FMOD_CHANNEL *channel;

	double threshold;
	double spectrum;
} track;
static track kick = { .dsp = 0, };
static track snare = { 0 };
static track bgmList[NUMBER_OF_BGM_TRACKS];
static int bgmCount = 0;
static double bgmDelay;
static int currentId = 0;


// SFX
static int sfxCount;
static FMOD_SOUND *sfxList[NUMBER_OF_SFX_TRACKS];

// For internal use
void _CheckResult(const char *debug);
void _CountDownBGM();


void AE_Init()
{
	// Initialise state
	bIsPlaying = true;
	bHasError = false;
	sfxCount = 0;

	// Setup the sound system
	result = FMOD_System_Create(&fmodSystem);
	_CheckResult("create");

	// Initialise sound system
	if (!bHasError) result = FMOD_System_Init(fmodSystem, NUMBER_OF_CHANNELS, FMOD_INIT_NORMAL, 0);
	_CheckResult("initialising");

	// LOAD TRACKS
	AE_LoadTrack("..//Kick.wav", KICK);
	AE_LoadTrack("..//Snare.wav", SNARE);
	AE_LoadTrack("..//Melody.wav", BGM);
	AE_StartBGMWithDelay(0, 2.17);
}

void AE_LoadTrack(const char *path, TRACKTYPE type)
{
	switch (type)
	{
	case KICK:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_DEFAULT, 0, &kick.sound);
		kick.threshold = .0001;
		break;
	case SNARE:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_DEFAULT, 0, &snare.sound);
		snare.threshold = .0001;
		break;
	case BGM:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_DEFAULT, 0, &bgmList[bgmCount].sound);
		bgmCount++;
		break;
	case SFX:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_LOOP_NORMAL, 0, &sfxList[sfxCount]);
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

	// Kick
	FMOD_System_CreateDSPByType(fmodSystem, FMOD_DSP_TYPE_FFT, &kick.dsp);
	FMOD_System_PlaySound(fmodSystem, kick.sound, 0, true, &kick.channel);
	FMOD_Channel_AddDSP(kick.channel, 0, kick.dsp);
	FMOD_DSP_SetActive(kick.dsp, true);

	// Snare
	FMOD_System_CreateDSPByType(fmodSystem, FMOD_DSP_TYPE_FFT, &snare.dsp);
	FMOD_System_PlaySound(fmodSystem, snare.sound, 0, true, &snare.channel);
	FMOD_Channel_AddDSP(snare.channel, 0, snare.dsp);
	FMOD_DSP_SetActive(snare.dsp, true);

	// Bgm
	FMOD_System_CreateDSPByType(fmodSystem, FMOD_DSP_TYPE_FFT, &bgmList[id].dsp);
	FMOD_System_PlaySound(fmodSystem, bgmList[id].sound, 0, true, &bgmList[id].channel);
	FMOD_Channel_AddDSP(bgmList[id].channel, 0, bgmList[id].dsp);
	FMOD_DSP_SetActive(bgmList[id].dsp, true);
}

int AE_GetFrequency(TRACKTYPE type)
{
	int result;
	switch (type)
	{
	case KICK:
		result = kick.spectrum > kick.threshold ? 1 : 0;
		break;
	case SNARE:
		result = snare.spectrum > snare.threshold ? 1 : 0;
		break;
	}
	return result;
}

void AE_Update()
{
	result = FMOD_System_Update(fmodSystem);
	_CheckResult("updating");

	FMOD_DSP_GetParameterData(kick.dsp, FMOD_DSP_FFT_SPECTRUMDATA, &kick.dspFFT, 0, 0, 0);
	if (kick.dspFFT->spectrum[0])
		kick.spectrum = (double)((*(kick.dspFFT->spectrum[0]) + *(kick.dspFFT->spectrum[1])) / 2);
	FMOD_DSP_GetParameterData(snare.dsp, FMOD_DSP_FFT_SPECTRUMDATA, &snare.dspFFT, 0, 0, 0);
	if (snare.dspFFT->spectrum[0])
		snare.spectrum = (double)((*(snare.dspFFT->spectrum[0]) + *(snare.dspFFT->spectrum[1])) / 2);
	//_UpdateSpectrum(kick.dsp, kick.dspFFT, &kick.spectrum);
	//_UpdateSpectrum(snare.dsp, snare.dspFFT, &snare.spectrum);


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
	FMOD_Sound_Release(kick.sound);
	FMOD_Sound_Release(snare.sound);
	for (int i = 0; i < bgmCount; i++)
		FMOD_Sound_Release(bgmList[i].sound);
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

		FMOD_Channel_SetPaused(kick.channel, false);
		FMOD_Channel_SetPaused(snare.channel, false);
		FMOD_Channel_SetPaused(bgmList[0].channel, false);
	}
}

void AE_SetBGMVolume(float volume, TRACKTYPE type)
{
	switch (type)
	{
	case KICK:
		FMOD_Channel_SetVolume(kick.channel, volume);
	case SNARE:
		FMOD_Channel_SetVolume(snare.channel, volume);
	case BGM:
		FMOD_Channel_SetVolume(bgmList[currentId].channel, volume);
		break;
	case SFX:
		//result = FMOD_ChannelGroup_SetVolume(sfxGroup, volume);
		break;
	}
	_CheckResult("setting volume");
}

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