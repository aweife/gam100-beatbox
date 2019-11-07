#include "Random.h"
#include <time.h>
#include <stdlib.h>


void Random_Init()
{
	srand(time(NULL));
}

int Random_Range(int min_n, int max_n)
{
	return rand() % (max_n - min_n + 1) + min_n;
}