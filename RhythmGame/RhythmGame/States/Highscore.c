#include "MainMenu.h"
#include <Windows.h>
#include "../Text/TextReader.h"
#include "StateMachine.h"
#include "Game.h"
#include"../UI/UI.h"

sprite HS;
sprite trophy;

void Highscore_EnterState()
{
	HS = Text_CreateSprite();
	Text_Init(&HS, "..//RhythmGame//$Resources//Highscore.txt");
	Text_Move(&HS,5, 10);

	trophy = Text_CreateSprite();
	Text_Init(&trophy, "..//RhythmGame//$Resources//trophy2.txt");
	Text_Move(&trophy,10,25);
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
	Text_Render(&trophy, 0, 0);
}

