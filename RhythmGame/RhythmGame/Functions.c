#include "Functions.h"

int reqExit = 0;

void Global_Exit()
{
	reqExit = 1;
}

int gameRunning()
{
	return (reqExit == 0);
}