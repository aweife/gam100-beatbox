#include "AudioEngine.h"
#include "Clock/Clock.h"

double magictimer = 0.0;
double magicnumber = 5.0f;

int main(void)
{
	AE_Init();
	AE_LoadTrack("..//yoursound.mp3", SFX);
	//AE_PlayOneShot(0, 0.5f);
	printf("Loaded bgm\n");
	magictimer = 0.0;
	while (true)
	{
		Clock_GameLoopStart();
		magictimer += Clock_GetDeltaTime(); 
		printf(" ");
;		if (magictimer > 2000.0)
		{
			printf("Playing bgm with 5 seconds delay\n");
			AE_StartBGMWithDelay(0, magicnumber);
			magictimer = -1000000000.0;
		}

		AE_Update();
	}


	return 0;
}