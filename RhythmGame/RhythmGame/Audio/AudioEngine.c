#include "AudioEngine.h"
#include "fmod.h"
#include "fmod_errors.h"
#include <stdlib.h>

#define NUMBER_OF_CHANNELS 20
#define MAX_NUMBER_OF_TRACKS 5

#define BEAT_THRESHOLD 0.0000000017


typedef struct track
{
	FMOD_DSP *dsp;
	FMOD_DSP_PARAMETER_FFT *dspFFT;
	FMOD_SOUND *sound;
	FMOD_CHANNEL *channel;
} track;

track tracks[MAX_NUMBER_OF_TRACKS] = { 0 };
FMOD_SOUND *sfx[1] = { 0 };

// FMOD-specific stuff
static FMOD_SYSTEM *_system;
static FMOD_RESULT result;


// For internal use
void _CheckResult(const char *debug);


void Audio_Create()
{
	// Create fmodsystem
	result = FMOD_System_Create(&_system);
	_CheckResult("create");
	// Initialise system
	result = FMOD_System_Init(_system, NUMBER_OF_CHANNELS, FMOD_INIT_NORMAL, 0);
	_CheckResult("init");


	// Create DSPs
	for (int i = 0; i < MAX_NUMBER_OF_TRACKS; i++)
	{
		FMOD_System_CreateDSPByType(_system, FMOD_DSP_TYPE_FFT, &tracks[i].dsp);
		FMOD_DSP_SetParameterInt(tracks[i].dsp, FMOD_DSP_FFT_WINDOWTYPE, FMOD_DSP_FFT_WINDOW_TRIANGLE);
		FMOD_DSP_SetParameterInt(tracks[i].dsp, FMOD_DSP_FFT_WINDOWSIZE, 1024);
	}
}

void Audio_Load(STAGE stage)
{
	switch (stage)
	{
	case MAINMENU:
		FMOD_System_CreateSound(_system, "..//RhythmGame//$Resources//KickMenu.wav", FMOD_LOOP_NORMAL | FMOD_CREATESAMPLE, 0, &tracks[0].sound);
		FMOD_System_CreateSound(_system, "..//RhythmGame//$Resources//SnareMenu.wav", FMOD_LOOP_NORMAL | FMOD_CREATESAMPLE, 0, &tracks[1].sound);
		FMOD_System_CreateSound(_system, "..//RhythmGame//$Resources//MelodyMenu.wav", FMOD_LOOP_NORMAL | FMOD_CREATESAMPLE, 0, &tracks[2].sound);
		break;
	case TUTORIAL:
		FMOD_System_CreateSound(_system, "..//RhythmGame//$Resources//KickMenu.wav", FMOD_LOOP_NORMAL | FMOD_CREATESAMPLE, 0, &tracks[0].sound);
		FMOD_System_CreateSound(_system, "..//RhythmGame//$Resources//SnareMenu.wav", FMOD_LOOP_NORMAL | FMOD_CREATESAMPLE, 0, &tracks[1].sound);
		FMOD_System_CreateSound(_system, "..//RhythmGame//$Resources//MelodyMenu.wav", FMOD_LOOP_NORMAL | FMOD_CREATESAMPLE, 0, &tracks[2].sound);
		break;
	case STAGEONE:
		FMOD_System_CreateSound(_system, "..//RhythmGame//$Resources//Snare1.wav", FMOD_LOOP_OFF | FMOD_CREATESAMPLE, 0, &tracks[0].sound);
		FMOD_System_CreateSound(_system, "..//RhythmGame//$Resources//Normal1.wav", FMOD_LOOP_OFF | FMOD_CREATESAMPLE, 0, &tracks[1].sound);
		FMOD_System_CreateSound(_system, "..//RhythmGame//$Resources//Laser1.wav", FMOD_LOOP_OFF | FMOD_CREATESAMPLE, 0, &tracks[2].sound);
		FMOD_System_CreateSound(_system, "..//RhythmGame//$Resources//Warning1.wav", FMOD_LOOP_OFF | FMOD_CREATESAMPLE, 0, &tracks[3].sound);
		FMOD_System_CreateSound(_system, "..//RhythmGame//$Resources//Melody1.wav", FMOD_LOOP_OFF | FMOD_CREATESAMPLE, 0, &tracks[4].sound);
		break;
	}
}

void Audio_Unload()
{
	for (int i = 0; i < MAX_NUMBER_OF_TRACKS; i++)
		if (tracks[i].sound)
			FMOD_Sound_Release(tracks[i].sound);
}

void Audio_PlayOneShot(int id, float volume)
{
	// Play the track paused
	FMOD_CHANNEL *c;
	result = FMOD_System_PlaySound(_system, sfx[id], 0, true, &c);
	_CheckResult("playing");

	// Set the volume of the one shot
	if (volume < 0.0f || volume > 1.0f) return;
	result = FMOD_Channel_SetVolume(c, volume);
	_CheckResult("setting volume");

	// Unpause the track
	result = FMOD_Channel_SetPaused(c, false);
	_CheckResult("playing");
}

void Audio_PlayBGM(STAGE stage)
{
	switch (stage)
	{
	case MAINMENU:
		for (int i = 0; i < 3; i++)
		{
			FMOD_System_PlaySound(_system, tracks[i].sound, 0, false, &tracks[i].channel);
			FMOD_Channel_AddDSP(tracks[i].channel, 0, tracks[i].dsp);
			FMOD_DSP_SetActive(tracks[i].dsp, true);
			FMOD_DSP_GetParameterData(tracks[i].dsp, FMOD_DSP_FFT_SPECTRUMDATA, (void **)&tracks[i].dspFFT, 0, 0, 0);
		}
		break;
	case TUTORIAL:
		for (int i = 0; i < 3; i++)
		{
			FMOD_System_PlaySound(_system, tracks[i].sound, 0, false, &tracks[i].channel);
			FMOD_Channel_AddDSP(tracks[i].channel, 0, tracks[i].dsp);
			FMOD_DSP_SetActive(tracks[i].dsp, true);
			FMOD_DSP_GetParameterData(tracks[i].dsp, FMOD_DSP_FFT_SPECTRUMDATA, (void **)&tracks[i].dspFFT, 0, 0, 0);
		}
		break;
	case STAGEONE:
		for (int i = 0; i < 5; i++)
		{
			FMOD_System_PlaySound(_system, tracks[i].sound, 0, false, &tracks[i].channel);
			FMOD_Channel_AddDSP(tracks[i].channel, 0, tracks[i].dsp);
			FMOD_DSP_SetActive(tracks[i].dsp, true);
			FMOD_DSP_GetParameterData(tracks[i].dsp, FMOD_DSP_FFT_SPECTRUMDATA, (void **)&tracks[i].dspFFT, 0, 0, 0);
		}
		break;
	}
}

bool Audio_GetSpectrum(int id)
{
	// Im not sure if I will stil get access violation
	if (tracks[id].dspFFT->spectrum[0])
	{
		// The value we get back
		return (tracks[id].dspFFT->spectrum[0][1] > BEAT_THRESHOLD) ? true : false;
		//return tracks[id].dspFFT->spectrum[0][1];
	}
}

void Audio_Update()
{
	FMOD_System_Update(_system);
}

void Audio_Shutdown()
{
	// Unload all sounds
	Audio_Unload();

	// All channels stop playing and released, main system too
	result = FMOD_System_Release(_system);
	_CheckResult("system shutdown");
}

void _CheckResult(const char *debug)
{
	if (result == FMOD_OK) return;

	printf("FMOD ERROR during %s! (%d) %s\n", debug, result, FMOD_ErrorString(result));
}