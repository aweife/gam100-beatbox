#include <stdio.h>
#include "AudioEngine.h"
#include <Windows.h>

int main(void)
{
	AE_Init();
	AE_LoadTrack("..\\yoursound.mp3", BGM);
	AE_LoadTrack("..\\swish.wav", SFX);
	AE_Play(0, BGM);
	AE_SetVolume(0.5f, BGM);
	Sleep(3000);
	AE_Play(0, SFX);
	Sleep(300);
	AE_Play(0, SFX);
	Sleep(300);
	AE_Play(0, SFX);
	Sleep(300);
	AE_Play(0, SFX);
	Sleep(300);
	AE_Play(0, SFX);
	AE_Play(0, SFX);
	while (1);


	return 0;
}