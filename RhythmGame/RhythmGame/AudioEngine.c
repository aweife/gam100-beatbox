#include "AudioEngine.h"

static int on; //is sound on?
static int possible; //is it possible to play sound?
static char *currentSound; //currently played sound

//FMOD-specific stuff
static FMOD_RESULT result;
static FMOD_SYSTEM *fmodsystem;
static FMOD_SOUND *sound;
static FMOD_CHANNEL *channel;
static FMOD_CHANNEL *sfx;
static FMOD_CHANNELGROUP *master;

//initialises sound
void initialise(void) {
	on = 1;
	possible = 1;

	//create the sound system. If fails, sound is set to impossible
	result = FMOD_System_Create(&fmodsystem);
	if (result != FMOD_OK) possible = 0;
	//if initialise the sound system. If fails, sound is set to impossible
	if (possible) result = FMOD_System_Init(fmodsystem, 2, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) possible = 0;
	//sets initial sound volume (mute)
	if (possible) FMOD_Channel_SetVolume(channel, 0.5f);
}

//sets the actual playing sound's volume
void setVolume(float v) {
	if (possible && on && v >= 0.0f && v <= 1.0f) {
		FMOD_Channel_SetVolume(channel, v);
	}
}

//loads a soundfile
void load(const char* filename) {
	currentSound = (char*)filename;
	if (possible && on) {
		result = FMOD_Sound_Release(sound);
		result = FMOD_System_CreateStream(fmodsystem, currentSound, FMOD_CREATESTREAM, 0, &sound);
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
		result = FMOD_Sound_Release(sound);
	}
}

//plays a sound (no argument to leave pause as dafault)
void play(int pause) 
{
	if (possible && on) {
		result = FMOD_System_PlaySound(fmodsystem, sound, NULL, pause, &channel);
		FMOD_Channel_SetMode(channel, FMOD_LOOP_NORMAL);
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
	FMOD_Channel_SetPaused(channel, pause);
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
	FMOD_Channel_GetPaused(channel, &p);
	FMOD_Channel_SetPaused(channel, !p);
}

//tells whether the sound is on or off
int getSound(void) 
{
	return on;
}