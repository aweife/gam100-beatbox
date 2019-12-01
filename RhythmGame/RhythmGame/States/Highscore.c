#define _CRT_SECURE_NO_DEPRECATE
#include "MainMenu.h"
#include <Windows.h>
#include <stdlib.h>
#include<stdio.h>
#include "../Text/TextReader.h"
#include "StateMachine.h"
#include"Highscore.h"
#include "Game.h"
#include"../UI/UI.h"

sprite HS;
sprite trophyman;
sprite HSui;
Pinfo *PI;
Pinfo Topplayers[5];
Pinfo Highestscorers;
FILE* readScoreFile;

Vector2d infopositioning[5][2];
int NameScorey;
static char buffer[200];

static int charcount;
static int scorecount;
static int pcount;
static int nameiterator = 0;
static int scoreiterator = 0;
static int HSstate = 0;



void _Init_players()
{

	pcount = _Determineamountofplayers("..//RhythmGame//$Resources//scores.txt");
	PI = (Pinfo*)malloc(sizeof(Pinfo) * pcount);

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

	readScoreFile = fopen(path, "r");
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

	for (int j = 0;j < pcount; j++)
	{
		PI[j].score = 0;
		scoreiterator = 0;
		for (int i = 0; i < 10; i++)
		{
			PI[j].playerscore[i].printsprite = Text_CreateSprite();
			Text_InitArray(&PI[j].playerscore[i].printsprite, "..//RhythmGame//$Resources//numbers.txt", 0);
			PI[j].playerscore[i].score = 'n';
			PI[j].playerscore[i].visible = 0;

			Text_Move(&PI[j].playerscore[i].printsprite, infopositioning[j][0].x + scoreiterator, infopositioning[j][0].y);
			scoreiterator += 4;
		}
	}
}

void _Init_Name()
{
	nameiterator = 0;

	for (int j = 0; j < pcount; j++)
	{
		nameiterator = 0;
		for (int i = 0; i < 8; i++)
		{
			PI[j].playername[i].printsprite = Text_CreateSprite();
			Text_InitArray(&PI[j].playername[i].printsprite, "..//RhythmGame//$resources//Alphabets2.txt", 6);
			PI[j].playername[i].name = '0';
			PI[j].playername[i].visible = 0;

			Text_Move(&PI[j].playername[i].printsprite, infopositioning[j][1].x + nameiterator, infopositioning[j][1].y);
			nameiterator += 6;


		}

	}

}


void _determinePscore(Pinfo *p,int count)
{
	switch (count)
	{
	case 0:break;

	case 1: p->score = (p->playerscore[0].score) - '0';



	}
}


void _readandstoreinfo(Pinfo *p,char *path)
{
	HSstate = 0;
	charcount = 0;
	scorecount = 0;

	readScoreFile = fopen(path, "r");
		if (readScoreFile == NULL)
		{
			perror("Error opening file");
		}
		else {


			while (!feof(readScoreFile))
			{
				charcount = 0;
				scorecount = 0;

				fgets(buffer, 200, readScoreFile);

				for (int i = 0; i < 200; i++)
				{

					if (buffer[i] == '\n')
						break;

					if (!isdigit(buffer[i]) && buffer[i] != ' ' && buffer[i] != '\0')
					{
						(p+HSstate)->playername[charcount].name = buffer[i];
						(p+HSstate)->playername[charcount].visible = 1;
						charcount++;
					}

					else if (isdigit(buffer[i]) && buffer[i] != ' ' && buffer[i] != '\0')
					{
						(p+HSstate)->playerscore[scorecount].score = buffer[i];
						(p+HSstate)->playerscore[scorecount].visible = 1;
						scorecount++;
					}

				}


				HSstate++;
			}

			fclose(readScoreFile);
		}

}

void _determineprintingname(Pinfo *p)
{

	for (int i = 0; i < 8; i++)
	{
		switch (p->playername[i].name)
		{
		case 'a' :case 'A':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets.txt", 0);
			break;
		case 'b':case 'B':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets.txt", 1);
			break;
		case 'c':case 'C':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets.txt", 2);
			break;
		case 'd':case 'D':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets.txt", 3);
			break;
		case 'e':case 'E':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets.txt", 4);
			break;
		case 'f':case 'F':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets.txt", 5);
			break;
		case 'g':case 'G':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets.txt", 6);
			break;
		case 'h':case 'H':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets.txt", 7);
			break;
		case 'i':case 'I':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets.txt", 8);
			break;
		case 'j':case 'J':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets.txt", 9);
			break;
		case 'k':case 'K':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets2.txt", 0);
			break;
		case 'l':case 'L':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets2.txt", 1);
			break;
		case 'm':case 'M':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets2.txt", 2);
			break;
		case 'n':case 'N':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets2.txt", 3);
			break;
		case 'o':case 'O':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets2.txt", 4);
			break;
		case 'p':case 'P':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets2.txt", 5);
			break;
		case 'q':case 'Q':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets2.txt", 6);
			break;
		case 'r':case 'R':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets2.txt", 7);
			break;
		case 's':case 'S':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets2.txt", 8);
			break;
		case 't':case 'T':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets2.txt", 9);
			break;
		case 'u':case 'U':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets3.txt", 0);
			break;
		case 'v':case 'V':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets3.txt", 1);
			break;
		case 'w':case 'W':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets3.txt", 2);
			break;
		case 'x':case 'X':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets3.txt", 3);
			break;
		case 'y':case 'Y':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets3.txt", 4);
			break;
		case 'z':case 'Z':
			Text_InitArray(&p->playername[i].printsprite, "..//RhythmGame//$resources//Alphabets3.txt", 5);
			break;

		}

	
	}

}

void _determineprintingscore(Pinfo* p)
{

	for (int i = 0; i < 10; i++)
	{
		switch (p->playerscore[i].score)
		{
		case '0':
			Text_InitArray(&p->playerscore[i].printsprite, "..//RhythmGame//$resources//numbers.txt", 0);
			break;
		case '1':
			Text_InitArray(&p->playerscore[i].printsprite, "..//RhythmGame//$resources//numbers.txt", 1);
			break;
		case '2':
			Text_InitArray(&p->playerscore[i].printsprite, "..//RhythmGame//$resources//numbers.txt", 2);
			break;
		case '3':
			Text_InitArray(&p->playerscore[i].printsprite, "..//RhythmGame//$resources//numbers.txt", 3);
			break;
		case '4':
			Text_InitArray(&p->playerscore[i].printsprite, "..//RhythmGame//$resources//numbers.txt", 4);
			break;
		case '5':
			Text_InitArray(&p->playerscore[i].printsprite, "..//RhythmGame//$resources//numbers.txt", 5);
			break;
		case '6':
			Text_InitArray(&p->playerscore[i].printsprite, "..//RhythmGame//$resources//numbers.txt", 6);
			break;
		case '7':
			Text_InitArray(&p->playerscore[i].printsprite, "..//RhythmGame//$resources//numbers.txt", 7);
			break;
		case '8':
			Text_InitArray(&p->playerscore[i].printsprite, "..//RhythmGame//$resources//numbers.txt", 8);
			break;
		case '9':
			Text_InitArray(&p->playerscore[i].printsprite, "..//RhythmGame//$resources//numbers.txt", 9);
			break;

		}


	}

}

void _determinetopplayers(Pinfo *p)
{
	for (int i = 0; i < pcount; i++)
	{
  
	}
}




void Highscore_EnterState()
{

	HS = Text_CreateSprite();
	Text_Init(&HS, "..//RhythmGame//$Resources//Highscore2.txt");
	Text_Move(&HS,5, 55);

	trophyman = Text_CreateSprite();
	Text_Init(&trophyman, "..//RhythmGame//$Resources//trophyman1.txt");
	Text_Move(&trophyman,80,10);
	
	HSui = Text_CreateSprite();
	Text_Init(&HSui,"..//RhythmGame//$Resources//scoreinfo.txt");
	Text_Move(&HSui,40,65);
	_Init_positions();
	_Init_players();
	_Init_Name();
	_Init_Numbers();


     _readandstoreinfo(&PI[0],"..//RhythmGame//$Resources//scores.txt");

	 for (int i = 0; i < pcount; i++)
	 {
		 _determineprintingname(&PI[i]);
		 _determineprintingscore(&PI[i]);
	 }

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
	Text_Render(&HSui, 0, 0);

	for (int j = 0; j < pcount; j++)
	{
		for (int i = 0; i < 10; i++)
		{
			if (PI[j].playerscore[i].visible == 1)
				Text_Render(&PI[j].playerscore[i].printsprite, 0, 0);
		}

		for (int k = 0; k < 8; k++)
		{
			if (PI[j].playername[k].visible == 1)
				Text_Render(&PI[j].playername[k].printsprite, 0, 0);
		}
	}
	//}
}
//Write Score to File
//void _InputScore(char* path, int Get_Score())
//{
//	writeScoreFile = fopen(path, "a");
//	if (writeScoreFile == NULL)
//	{
//		perror("Error opening file");
//	}
//	else {
//		fprintf(writeScoreFile, "Player Score: %d\n", Get_Score());
//		fclose(writeScoreFile);
//	}
//}
