#include "AudioEngine.h"
#include "fmod.h"
#include "fmod_errors.h"
#include <stdbool.h>

#define NUMBER_OF_BGM_TRACKS 1
#define NUMBER_OF_SFX_TRACKS 1
#define NUMBER_OF_CHANNELS 20

typedef struct track
{
	FMOD_DSP *dsp;
	FMOD_DSP_PARAMETER_FFT *dspFFT;
	FMOD_SOUND *sound;
	FMOD_CHANNEL *channel;
	double spectrum;
} track;

// State
static bool bIsPlaying;
static bool bHasError;

// FMOD-specific stuff
static FMOD_RESULT result;
static FMOD_SYSTEM *fmodSystem;

// BGM
static track kick = { 0 };
static track snare = { 0 };
static track bgmList[NUMBER_OF_BGM_TRACKS];
static int bgmCount = 0;
static double bgmDelay;
static int currentId = 0;
static bool startFadeOut = false;
static double fadeTimer = 0;
static double initialFadeValue = 0;


// SFX
static int sfxCount;
static FMOD_SOUND *sfxList[NUMBER_OF_SFX_TRACKS];

// For internal use
void _CheckResult(const char *debug);
void _CountDownBGM();
void _FadeOutBGM();


void Audio_Init()
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
	Audio_Load("../RhythmGame//$Resources//Kick.wav", KICK);
	Audio_Load("..//RhythmGame//$Resources//Snare.wav", SNARE);
	Audio_Load("..//RhythmGame//$Resources//Melody.wav", BGM);
	Audio_PlayBGMWithDelay(0, .000001);
}

void Audio_Load(const char *path, TRACKTYPE type)
{
	switch (type)
	{
	case KICK:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_DEFAULT, 0, &kick.sound);
		break;
	case SNARE:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_DEFAULT, 0, &snare.sound);
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

void Audio_PlayOneShot(int id, float volume)
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

void Audio_PlayBGMWithDelay(int id, double delay)
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

void Audio_FadeOutBGM(double time)
{
	startFadeOut = true;
	fadeTimer = time;
	initialFadeValue = time;
}

double Audio_GetFrequency(TRACKTYPE type)
{
	switch (type)
	{
	case KICK:
		return kick.spectrum;
	case SNARE:
		return snare.spectrum;
	}
}

void Audio_Update()
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

	// If FadeOutBGM has been called
	if (startFadeOut)
		_FadeOutBGM();
}

void Audio_Shutdown()
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

void Audio_SetBGMVolume(float volume, TRACKTYPE type)
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

void _FadeOutBGM()
{
	fadeTimer -= Clock_GetDeltaTime();
	double volume = fadeTimer / initialFadeValue;

	FMOD_Channel_SetVolume(snare.channel, volume);
	FMOD_Channel_SetVolume(kick.channel, volume);
	FMOD_Channel_SetVolume(bgmList[0].channel, volume);

	if (volume < 0.0)
	{
		startFadeOut = false;
		fadeTimer = 0.0;
		initialFadeValue = 0.0;

		FMOD_Channel_SetPaused(kick.channel, true);
		FMOD_Channel_SetPaused(snare.channel, true);
		FMOD_Channel_SetPaused(bgmList[0].channel, true);
	}
}
