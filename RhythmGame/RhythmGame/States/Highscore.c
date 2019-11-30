#include "MainMenu.h"
#include <Windows.h>
#include "../Text/TextReader.h"
#include "StateMachine.h"
#include "Game.h"
#include"../UI/UI.h"

sprite HS;
sprite trophyman;
sprite trophyman2;

void ReadfromTextHS();

void Highscore_EnterState()
{
	HS = Text_CreateSprite();
	Text_Init(&HS, "..//RhythmGame//$Resources//Highscore2.txt");
	Text_Move(&HS,5, 5);

	trophyman = Text_CreateSprite();
	Text_Init(&trophyman, "..//RhythmGame//$Resources//trophyman3.txt");
	Text_Move(&trophyman,10,25);
}

void Highscore_ExitState()
{
	Text_Cleanup(&HS);
}

void Highscore_ProcessInput()
{

}

void Highscore_Update()
{

}

void Highscore_Render()
{
	Text_Render(&HS,0,0);
	Text_Render(&trophyman, 0, 0);
}

void _ReadfromTextHS()
{


}