#include "../Text/TextReader.h"

sprite hearts[10];
int StartX;
int StartY;


void UI_Init()
{
	StartX = 5;
	StartY = 5;
	for (int i = 0; i < 10; i++)
	{
		hearts[i] = Text_CreateSprite();
		Text_Init(&hearts[i], "..//RhythmGame//$Resources//Health2.txt");
		Text_Move(&hearts[i], StartX, StartY);
	}
	
	
}

void UI_Update()
{


}

void UI_Render()
{   
	for (int i = 0; i < 10; i++)
	{
		Text_Render(&hearts[i]);
	}
}

