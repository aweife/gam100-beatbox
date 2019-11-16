#include "Text/TextReader.h"
#include "UI/UI.h"
sprite hearts[10];
int StartX;
int StartY;


void UI_CreateHearts()
{
	StartX = 50;
	StartY = 50;

	for (int i = 0; i < 10; i++)
	{
		hearts[i] = Text_CreateSprite();
		Text_Init(&hearts[i], "..//RhythmGame//$Resources//health.txt");

		Text_Moveenemy(&hearts[i], StartX, StartY);

		StartY += 12;
    }

}

void UI_Update()
{


}

void UI_Render()
{   
	for (int i = 0; i < 10; i++)
	{
		Text_RenderEnemy(&hearts[i]);
	}
}

