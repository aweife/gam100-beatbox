#pragma once

typedef struct Phighscore
{
	int visible;
	char score;
	sprite printsprite;
}Phighscore;

typedef struct Pname
{
	int visible;
    char name;
	sprite printsprite;
}Pname;

typedef struct Pinfo
{
	Phighscore playerscore[10];
	Pname playername[8];
	int score;

}Pinfo;


extern void Highscore_EnterState();
extern void Highscore_ExitState();
extern void Highscore_ProcessInput();
extern void Highscore_Update();
extern void Highscore_Render();