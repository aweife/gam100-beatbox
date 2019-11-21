#include "AudioEngine.h"
#include "fmod.h"
#include "fmod_errors.h"

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

// For menu and tutorial
static track kick = { 0 };

// Projectile
static track projectileList[1] = { 0 };
static int projectileCount = 0;

// Laser
static track warningList[1] = { 0 };
static int warningCount = 0;
static track laserList[1] = { 0 };
static int laserCount = 0;

// Enemy movement
static track snareList[2] = { 0 };
static int snareCount = 0;

// BGM
static track bgmList[2];
static int bgmCount = 0;

// For starting music
static double startDelay;
static bool startFadeOut = false;
static double fadeTimer = 0;
static double initialFadeValue = 0;
int currentId = 0; // Select music

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
	result = FMOD_System_Init(fmodSystem, NUMBER_OF_CHANNELS, FMOD_INIT_NORMAL, 0);
	_CheckResult("init");

	// LOAD TRACKS
	// Kick
	Audio_Load("../RhythmGame//$Resources//Kick.wav", KICK);
	// Snare
	Audio_Load("..//RhythmGame//$Resources//Snare1.wav", SNARE);
	Audio_Load("..//RhythmGame//$Resources//Snare.wav", SNARE);
	// Melody
	Audio_Load("..//RhythmGame//$Resources//Melody1.wav", BGM);
	Audio_Load("..//RhythmGame//$Resources//Melody.wav", BGM);
	// Projectile
	Audio_Load("..//RhythmGame//$Resources//Normal1.wav", PROJECTILE);
	// Warning
	Audio_Load("..//RhythmGame//$Resources//Warning1.wav", WARNING);
	// Laser
	Audio_Load("..//RhythmGame//$Resources//Laser1.wav", LASER);
}

void Audio_Load(const char *path, TRACKTYPE type)
{
	switch (type)
	{
	case KICK:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_DEFAULT, 0, &kick.sound);
		break;
	case SNARE:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_DEFAULT, 0, &snareList[snareCount].sound);
		snareCount++;
		break;
	case BGM:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_LOOP_NORMAL, 0, &bgmList[bgmCount].sound);
		bgmCount++;
		break;
	case SFX:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_LOOP_NORMAL, 0, &sfxList[sfxCount]);
		sfxCount++;
		break;
	case PROJECTILE:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_DEFAULT, 0, &projectileList[projectileCount].sound);
		projectileCount++;
		break;
	case WARNING:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_DEFAULT, 0, &warningList[warningCount].sound);
		warningCount++;
		break;
	case LASER:
		result = FMOD_System_CreateSound(fmodSystem, path, FMOD_DEFAULT, 0, &laserList[laserCount].sound);
		laserCount++;
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

void Audio_PlayBGMWithDelay(double delay, STAGE stage)
{
	startDelay = delay * 1000.0;

	switch (stage)
	{
	case MAINMENU:
		currentId = 1;
		break;
	case TUTORIAL:
		currentId = 1;
		break;
	case STAGEONE:
		currentId = 0;
		break;
	}

	if (currentId == 1)
	{
		// Kick
		FMOD_System_CreateDSPByType(fmodSystem, FMOD_DSP_TYPE_FFT, &kick.dsp);
		FMOD_System_PlaySound(fmodSystem, kick.sound, 0, true, &kick.channel);
		FMOD_Channel_AddDSP(kick.channel, 0, kick.dsp);
		FMOD_DSP_SetActive(kick.dsp, true);
	}

	// Snare
	FMOD_System_CreateDSPByType(fmodSystem, FMOD_DSP_TYPE_FFT, &snareList[currentId].dsp);
	FMOD_System_PlaySound(fmodSystem, snareList[currentId].sound, 0, true, &snareList[currentId].channel);
	FMOD_Channel_AddDSP(snareList[currentId].channel, 0, snareList[currentId].dsp);
	FMOD_DSP_SetActive(snareList[currentId].dsp, true);

	// Bgm
	FMOD_System_CreateDSPByType(fmodSystem, FMOD_DSP_TYPE_FFT, &bgmList[currentId].dsp);
	FMOD_System_PlaySound(fmodSystem, bgmList[currentId].sound, 0, true, &bgmList[currentId].channel);
	FMOD_Channel_AddDSP(bgmList[currentId].channel, 0, bgmList[currentId].dsp);
	FMOD_DSP_SetActive(bgmList[currentId].dsp, true);

	if (currentId == 1) return;

	// Projectile
	FMOD_System_CreateDSPByType(fmodSystem, FMOD_DSP_TYPE_FFT, &projectileList[currentId].dsp);
	FMOD_System_PlaySound(fmodSystem, projectileList[currentId].sound, 0, true, &projectileList[currentId].channel);
	FMOD_Channel_AddDSP(projectileList[currentId].channel, 0, projectileList[currentId].dsp);
	FMOD_DSP_SetActive(projectileList[currentId].dsp, true);

	// Warning
	FMOD_System_CreateDSPByType(fmodSystem, FMOD_DSP_TYPE_FFT, &warningList[currentId].dsp);
	FMOD_System_PlaySound(fmodSystem, warningList[currentId].sound, 0, true, &warningList[currentId].channel);
	FMOD_Channel_AddDSP(warningList[currentId].channel, 0, warningList[currentId].dsp);
	FMOD_DSP_SetActive(warningList[currentId].dsp, true);

	// Laser
	FMOD_System_CreateDSPByType(fmodSystem, FMOD_DSP_TYPE_FFT, &laserList[currentId].dsp);
	FMOD_System_PlaySound(fmodSystem, laserList[currentId].sound, 0, true, &laserList[currentId].channel);
	FMOD_Channel_AddDSP(laserList[currentId].channel, 0, laserList[currentId].dsp);
	FMOD_DSP_SetActive(laserList[currentId].dsp, true);
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
		return snareList[currentId].spectrum;
	case PROJECTILE:
		return projectileList[currentId].spectrum;
	case WARNING:
		return warningList[currentId].spectrum;
	case LASER:
		return laserList[currentId].spectrum;
	case BGM:
		return bgmList[currentId].spectrum;
	}
}

void Audio_Update()
{
	FMOD_System_Update(fmodSystem);

	//FMOD_DSP_GetParameterData(kick.dsp, FMOD_DSP_FFT_SPECTRUMDATA, &kick.dspFFT, 0, 0, 0);
	//if (kick.dspFFT->spectrum[0])
	//	kick.spectrum = (double)((*(kick.dspFFT->spectrum[0]) + *(kick.dspFFT->spectrum[1])) / 2);
	FMOD_DSP_GetParameterData(snareList[currentId].dsp, FMOD_DSP_FFT_SPECTRUMDATA, &snareList[currentId].dspFFT, 0, 0, 0);
	if (snareList[currentId].dspFFT)
		if (snareList[currentId].dspFFT->spectrum[0])
			snareList[currentId].spectrum = (double)((*(snareList[currentId].dspFFT->spectrum[0]) + *(snareList[currentId].dspFFT->spectrum[1])) / 2);
	FMOD_DSP_GetParameterData(projectileList[currentId].dsp, FMOD_DSP_FFT_SPECTRUMDATA, &projectileList[currentId].dspFFT, 0, 0, 0);
	if (projectileList[currentId].dspFFT)
		if (projectileList[currentId].dspFFT->spectrum[0])
			projectileList[currentId].spectrum = (double)((*(projectileList[currentId].dspFFT->spectrum[0]) + *(projectileList[currentId].dspFFT->spectrum[1])) / 2);
	FMOD_DSP_GetParameterData(warningList[currentId].dsp, FMOD_DSP_FFT_SPECTRUMDATA, &warningList[currentId].dspFFT, 0, 0, 0);
	if (warningList[currentId].dspFFT)
		if (warningList[currentId].dspFFT->spectrum[0])
			warningList[currentId].spectrum = (double)((*(warningList[currentId].dspFFT->spectrum[0]) + *(warningList[currentId].dspFFT->spectrum[1])) / 2);
	FMOD_DSP_GetParameterData(laserList[currentId].dsp, FMOD_DSP_FFT_SPECTRUMDATA, &laserList[currentId].dspFFT, 0, 0, 0);
	if (laserList[currentId].dspFFT)
		if (laserList[currentId].dspFFT->spectrum[0])
			laserList[currentId].spectrum = (double)((*(laserList[currentId].dspFFT->spectrum[0]) + *(laserList[currentId].dspFFT->spectrum[1])) / 2);
	FMOD_DSP_GetParameterData(bgmList[currentId].dsp, FMOD_DSP_FFT_SPECTRUMDATA, &bgmList[currentId].dspFFT, 0, 0, 0);
	if (bgmList[currentId].dspFFT)
		if (bgmList[currentId].dspFFT->spectrum[0])
			bgmList[currentId].spectrum = (double)((*(bgmList[currentId].dspFFT->spectrum[0]) + *(bgmList[currentId].dspFFT->spectrum[1])) / 2);
	//_UpdateSpectrum(kick.dsp, kick.dspFFT, &kick.spectrum);
	//_UpdateSpectrum(snare.dsp, snare.dspFFT, &snare.spectrum);


	// If StartBGMWithDelay has been called
	if (startDelay > 0.0)
		_CountDownBGM();

	// If FadeOutBGM has been called
	if (startFadeOut)
		_FadeOutBGM();
}

void Audio_Shutdown()
{
	// Free sounds in memory
	FMOD_Sound_Release(kick.sound);
	for (int i = 0; i < snareCount; i++)
		if (snareList[i].sound)
			FMOD_Sound_Release(snareList[i].sound);
	for (int i = 0; i < bgmCount; i++)
		if (bgmList[i].sound)
			FMOD_Sound_Release(bgmList[i].sound);
	for (int i = 0; i < projectileCount; i++)
		if (projectileList[i].sound)
			FMOD_Sound_Release(projectileList[i].sound);
	for (int i = 0; i < warningCount; i++)
		if (warningList[i].sound)
			FMOD_Sound_Release(warningList[i].sound);
	for (int i = 0; i < laserCount; i++)
		if (laserList[i].sound)
			FMOD_Sound_Release(laserList[i].sound);
	for (int i = 0; i < sfxCount; i++)
		if (sfxList[i])
			FMOD_Sound_Release(sfxList[i]);

	// All channels stop playing and released, main system too
	result = FMOD_System_Release(fmodSystem);
	_CheckResult("system shutdown");
}

void Audio_SetBGMVolume(float volume, TRACKTYPE type)
{
	switch (type)
	{
	case KICK:
		FMOD_Channel_SetVolume(kick.channel, volume);
		break;
	case SNARE:
		FMOD_Channel_SetVolume(snareList[currentId].channel, volume);
		break;
	case BGM:
		FMOD_Channel_SetVolume(bgmList[currentId].channel, volume);
		break;
	case PROJECTILE:
		FMOD_Channel_SetVolume(projectileList[currentId].channel, volume);
		break;
	case WARNING:
		FMOD_Channel_SetVolume(warningList[currentId].channel, volume);
		break;
	case LASER:
		FMOD_Channel_SetVolume(laserList[currentId].channel, volume);
		break;
	case SFX:
		//result = FMOD_ChannelGroup_SetVolume(sfxGroup, volume);
		break;
	}
}

void _CheckResult(const char *debug)
{
	if (result == FMOD_OK) return;

	bHasError = true;
	printf("FMOD ERROR during %s! (%d) %s\n", debug, result, FMOD_ErrorString(result));
}

void _CountDownBGM()
{
	startDelay -= Clock_GetDeltaTime();

	// Time the delay
	if (startDelay <= 0.0)
	{
		startDelay = 0.0;

		FMOD_Channel_SetPaused(kick.channel, false);
		FMOD_Channel_SetPaused(snareList[currentId].channel, false);
		FMOD_Channel_SetPaused(bgmList[currentId].channel, false);
		FMOD_Channel_SetPaused(projectileList[currentId].channel, false);
		FMOD_Channel_SetPaused(warningList[currentId].channel, false);
		FMOD_Channel_SetPaused(laserList[currentId].channel, false);
	}
}

void _FadeOutBGM()
{
	fadeTimer -= Clock_GetDeltaTime();
	double volume = fadeTimer / initialFadeValue;

	FMOD_Channel_SetVolume(kick.channel, volume);
	FMOD_Channel_SetVolume(snareList[currentId].channel, volume);
	FMOD_Channel_SetVolume(bgmList[currentId].channel, volume);

	if (volume < 0.0)
	{
		startFadeOut = false;
		fadeTimer = 0.0;
		initialFadeValue = 0.0;

		FMOD_Channel_SetPaused(kick.channel, true);
		FMOD_Channel_SetPaused(snareList[currentId].channel, true);
		FMOD_Channel_SetPaused(bgmList[currentId].channel, true);
	}
}
