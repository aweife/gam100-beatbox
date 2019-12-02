#include "MainMenu.h"
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "../Text/TextReader.h"
#include "StateMachine.h"
#include "Highscore.h"
#include "Game.h"
#include "../UI/UI.h"
#include "../Text/font.h"
#include "../Audio/AudioEngine.h"
#include <stdbool.h>

sprite HS;
sprite trophymanstate1;
sprite trophymanstate2;
sprite HSui;
Pinfo *PI;
Pinfo Topplayers[5];
Pinfo Highestscorers;
FILE *readScoreFile;

Vector2d infopositioning[5][2];
int NameScorey;
static char buffer[200];

static bool animateTrophyman = false;

static int charcount;
static int scorecount;
static int pcount;
static int nameiterator = 0;
static int scoreiterator = 0;
static int HSstate = 0;
static int digitCount = 0;

void _Init_players()
{

	pcount = _Determineamountofplayers("..//RhythmGame//$Resources//scores.txt");
	PI = (Pinfo *)malloc(sizeof(Pinfo) * pcount);

}

void _Init_positions()
{
	NameScorey = 75;
	for (int i = 0; i < 5; i++)
	{
		infopositioning[i][0].x = 125;
		infopositioning[i][0].y = NameScorey;

		infopositioning[i][1].x = 83;
		infopositioning[i][1].y = NameScorey;

		NameScorey += 10;
	}

}

int _Determineamountofplayers(char *path)
{
	int players = 0;

	fopen_s(&readScoreFile, path, "r");
	if (readScoreFile == NULL)
	{
		perror("Error opening file");
	}
	else {


		while (!feof(readScoreFile))
		{
			if (fgets(buffer, 200, readScoreFile))
				players++;

		}
		fclose(readScoreFile);
		return players;
	}


}

void _Init_Numbers()
{
	scoreiterator = 0;

	for (int j = 0; j < pcount; j++)
	{
		PI[j].score = 0;
		scoreiterator = 0;
		for (int i = 0; i < 8; i++)
		{
			PI[j].playerscore[i].printsprite = Text_CreateSprite();
			Text_InitArray(&PI[j].playerscore[i].printsprite, "..//RhythmGame//$Resources//numbers.txt", 0);
			PI[j].playerscore[i].visible = 0;
			Text_Move(&PI[j].playerscore[i].printsprite, infopositioning[j][0].x + scoreiterator, infopositioning[j][0].y);
			scoreiterator += 4;
		}
	}

	for (int j = 0; j < 5; j++)
	{
		PI[j].score = 0;
		scoreiterator = 0;
		for (int i = 0; i < 8; i++)
		{
			Topplayers[j].playerscore[i].printsprite = Text_CreateSprite();
			Text_InitArray(&Topplayers[j].playerscore[i].printsprite, "..//RhythmGame//$Resources//numbers.txt", 0);
			Topplayers[j].playerscore[i].visible = 0;
			Text_Move(&Topplayers[j].playerscore[i].printsprite, infopositioning[j][0].x + scoreiterator, infopositioning[j][0].y);
			scoreiterator += 4;
		}
	}
}

void _Move_placements()
{

	for (int j = 0; j < 5; j++)
	{

		nameiterator = 0;
		for (int i = 0; i < 3; i++)
		{

			Text_Move(&Topplayers[j].playername[i].printsprite, infopositioning[j][1].x + nameiterator, infopositioning[j][1].y);
			nameiterator += 6;
		}

		scoreiterator = 0;
		for (int i = 7; i >= 0; i--)
		{
			Text_Move(&Topplayers[j].playerscore[i].printsprite, infopositioning[j][0].x + scoreiterator, infopositioning[j][0].y);
			scoreiterator += 6;
		}



	}
}

void _Init_Name()
{


	for (int j = 0; j < pcount; j++)
	{
		nameiterator = 0;
		for (int i = 0; i < 3; i++)
		{
			PI[j].playername[i].printsprite = Text_CreateSprite();
			Text_InitArray(&PI[j].playername[i].printsprite, "..//RhythmGame//$resources//Alphabets2.txt", 6);
			PI[j].playername[i].visible = 0;

		}
	}

	for (int j = 0; j < 5; j++)
	{
		nameiterator = 0;
		for (int i = 0; i < 3; i++)
		{
			Topplayers[j].playername[i].printsprite = Text_CreateSprite();
			Text_InitArray(&Topplayers[j].playername[i].printsprite, "..//RhythmGame//$resources//Alphabets2.txt", 6);
			Topplayers[j].playername[i].visible = 0;

			Text_Move(&PI[j].playername[i].printsprite, infopositioning[j][1].x + nameiterator, infopositioning[j][1].y);
			nameiterator += 6;
		}
	}

}

void _readandstoreinfo(Pinfo *p, char *path)
{
	int letter1 = 0;
	int letter2 = 0;
	int letter3 = 0;
	int score = 0;


	HSstate = 0;
	charcount = 0;
	scorecount = 0;
	fopen_s(&readScoreFile, path, "r");
	if (readScoreFile == NULL)
	{
		perror("Error opening file");
	}
	else {


		while (!feof(readScoreFile))
		{
			fscanf_s(readScoreFile, "%d %d %d %d", &letter1, &letter2, &letter3, &(p + HSstate)->score);

			(p + HSstate)->playername[0].printsprite = Font_ConvertToSprite(letter1);
			(p + HSstate)->playername[0].visible = 1;
			(p + HSstate)->playername[1].printsprite = Font_ConvertToSprite(letter2);
			(p + HSstate)->playername[1].visible = 1;
			(p + HSstate)->playername[2].printsprite = Font_ConvertToSprite(letter3);
			(p + HSstate)->playername[2].visible = 1;

			for (int i = 0, digit = (p + HSstate)->score; digit > 0; i++, digit /= 10)
			{
				(p + HSstate)->playerscore[i].printsprite = Font_ConvertToSprite(26 + digit % 10);
				(p + HSstate)->playerscore[i].visible = 1;
			}


			HSstate++;
		}

		fclose(readScoreFile);
	}

}

void _determinetopplayers()
{
	int Pfound = 0;
	int maxnumber = PI[0].score;
	while (Pfound < 5)
	{
		maxnumber = 1;

		for (int k = 0; k < pcount; k++)
		{
			if (PI[k].score > maxnumber)
				maxnumber = PI[k].score;
		}

		for (int i = 0; i < pcount; i++)
		{
			if (PI[i].score == maxnumber)
			{
				Topplayers[Pfound] = PI[i];
				PI[i].score = 0;
			}

		}

		Pfound++;
	}


}

void Highscore_EnterState()
{

	HS = Text_CreateSprite();
	Text_Init(&HS, "..//RhythmGame//$Resources//Highscore2.txt");
	Text_Move(&HS, 5, 55);

	trophymanstate1 = Text_CreateSprite();
	Text_Init(&trophymanstate1, "..//RhythmGame//$Resources//trophyman1.txt");
	Text_Move(&trophymanstate1, 80, 10);

	trophymanstate2 = Text_CreateSprite();
	Text_Init(&trophymanstate2, "..//RhythmGame//$Resources//trophyman2.txt");
	Text_Move(&trophymanstate2, 79, 5);

	HSui = Text_CreateSprite();
	Text_Init(&HSui, "..//RhythmGame//$Resources//scoreinfo.txt");
	Text_Move(&HSui, 40, 65);
	_Init_positions();
	_Init_players();
	//_Init_Name();
	//_Init_Numbers();


	_readandstoreinfo(&PI[0], "..//RhythmGame//$Resources//scores.txt");

	_determinetopplayers();
	_Move_placements();
}

void Highscore_ExitState()
{
	Text_Cleanup(&HS);
	Text_Cleanup(&trophymanstate1);
	Text_Cleanup(&trophymanstate2);
	Text_Cleanup(&HSui);
}

bool keyDown;
void Highscore_ProcessInput()
{
	if (GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_RETURN) || GetAsyncKeyState(VK_UP) ||
		GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_RIGHT))
	{
		if (!keyDown)
			StateMachine_ChangeState(State_MainMenu);
		keyDown = true;
	}
	else
		keyDown = false;
}

void Highscore_Update()
{

}

void Highscore_Render()
{
	Text_Render(&HS, 0, 0);
	_RenderTrophymanAnimation();
	Text_Render(&HSui, 0, 0);

	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			if (Topplayers[j].playerscore[i].visible == 1)
				Text_Render(&Topplayers[j].playerscore[i].printsprite, 0, 0);
		}

		for (int k = 0; k < 3; k++)
		{
			if (Topplayers[j].playername[k].visible == 1)
				Text_Render(&Topplayers[j].playername[k].printsprite, 0, 0);
		}
	}
}

void _RenderTrophymanAnimation()
{
	animateTrophyman = !Audio_GetSpectrum(0);
	if (animateTrophyman == false)
	{
		Text_Render(&trophymanstate2, 0, 0);
	}

	if (animateTrophyman == true)
	{
		Text_Render(&trophymanstate1, 0, 0);
	}
}