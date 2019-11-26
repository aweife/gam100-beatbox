#include "fmod.h"
#include <stdlib.h>
#include <vector>

#define WINDOW_SIZE 1024

// FMOD
static FMOD_SYSTEM *_system;

// Projectile, enemy movement, laser
static FMOD_CHANNEL *_channel;
static FMOD_SOUND *_sound;
static FMOD_DSP *_dsp;
static float spectrum;
std::vector<int> numberOfSamples;

extern "C" void Audio_Create()
{
	// Setup the sound system
	FMOD_System_Create(&_system);
	// Initialise sound system
	FMOD_System_Init(_system, 20, FMOD_INIT_NORMAL, NULL);

	FMOD_System_CreateDSPByType(_system, FMOD_DSP_TYPE_FFT, &_dsp);
	FMOD_DSP_SetParameterInt(_dsp, FMOD_DSP_FFT_WINDOWTYPE, FMOD_DSP_FFT_WINDOW_TRIANGLE);
	FMOD_DSP_SetParameterInt(_dsp, FMOD_DSP_FFT_WINDOWSIZE, WINDOW_SIZE);
}

extern "C" void Audio_LoadSoundFromPath(const char *path, int i)
{
	FMOD_System_CreateStream(_system, path, FMOD_LOOP_OFF | FMOD_CREATESAMPLE, 0, &_sound);
}

extern "C" void Audio_Play()
{
	// Plays the bgm track
	FMOD_System_PlaySound(_system, _sound, 0, false, &_channel);

	// Create and attach the dsp to the channel
	FMOD_Channel_AddDSP(_channel, 0, _dsp);
	FMOD_DSP_SetActive(_dsp, true);
}

extern "C" void Audio_Update()
{
	// Call update per game tick to update info about track
	FMOD_System_Update(_system);
}

extern "C" int Audio_CreateSpectrum(int maxBars)
{
	// We use vector because C doesnt have a sorted list
	// Initialise list
	std::vector<int> frequencyOctaves;
	frequencyOctaves.push_back(0);

	// Logarithmic calculation to group the band of frequencies
	for (int i = 1; i < 13; ++i)
	{
		frequencyOctaves.push_back((int)((44100 / 2) / (float)pow(2, 12 - i)));
	}

	// Octaves
	int bandWidth = (44100 / WINDOW_SIZE);
	int bandsPerOctave = maxBars / 12;

	// Calculation number of FFT samples per bar
	numberOfSamples.clear();
	for (int octave = 0; octave < 12; ++octave)
	{
		int indexLow = frequencyOctaves[octave] / bandWidth;
		int indexHigh = (frequencyOctaves[octave + 1]) / bandWidth;
		int octavaIndexes = (indexHigh - indexLow);

		if (octavaIndexes > 0)
		{
			if (octavaIndexes <= bandsPerOctave)
			{
				for (int count = 0; count < octavaIndexes; ++count)
				{
					numberOfSamples.push_back(1);
				}
			}
			else
			{
				for (int count = 0; count < bandsPerOctave; ++count)
				{
					numberOfSamples.push_back(octavaIndexes / bandsPerOctave);
				}
			}
		}
	}

	// Return the bars that are useful
	return numberOfSamples.size();
}

extern "C" void Audio_GetSpectrum(float *spectrum)
{
	// Not sure if I should use a local dspFFT for this, check back later
	FMOD_DSP_PARAMETER_FFT *dspFFT = NULL;
	
	// This is the core function to read the audio
	FMOD_DSP_GetParameterData(_dsp, FMOD_DSP_FFT_SPECTRUMDATA, (void **)&dspFFT, 0, 0, 0);

	if (dspFFT)
	{
		// Only read half of the spectrum as FFT is reversed 
		int length = dspFFT->length / 2;
		int numChannels = dspFFT->numchannels;

		if (length > 0)
		{
			int indexFFT = 0;

			// For each bar we have in numberOfSamples
			for (int index = 0; index < numberOfSamples.size(); ++index)
			{
				// For each frequency
				for (int frec = 0; frec < numberOfSamples[index]; ++frec)
				{
					// Update one index
					for (int channel = 0; channel < numChannels; ++channel)
					{
						spectrum[index] += dspFFT->spectrum[channel][indexFFT];
					}
					++indexFFT;
				}

				// The value we get back divided by overall
				spectrum[index] /= (float)(numberOfSamples[index] * numChannels);
			}
		}
	}
}
