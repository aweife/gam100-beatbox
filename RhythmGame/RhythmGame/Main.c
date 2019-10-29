#include <stdio.h>
#include "AudioEngine.h"
#include <Windows.h>

int main(void)
{
	AE_Init();
	AE_LoadBGM("..\\yoursound.mp3");
	AE_LoadSFX("..\\swish.wav");
	AE_PlayBGM(0);
	Sleep(3000);
	AE_PlaySFX(0);
	Sleep(300);
	AE_PlaySFX(0);
	Sleep(300);
	AE_PlaySFX(0);
	Sleep(300);
	AE_PlaySFX(0);
	Sleep(300);
	AE_PlaySFX(0);
	while (1);


	return 0;
}