#include <stdio.h>
#include "AudioEngine.h"
#include <Windows.h>

int main(void)
{
	initialise();
	load("..\\yoursound.mp3");
	play(0);
	while (1);


	return 0;
}