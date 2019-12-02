#pragma once

typedef struct Phighscore
{
	int visible;
	sprite printsprite;
}Phighscore;

typedef struct Pname
{
	int visible;
	sprite printsprite;
}Pname;

typedef struct Pinfo
{
	Phighscore playerscore[8];
	Pname playername[3];
	int score;

}Pinfo;

#define HIGHSCORE_PATH "$HighScores//highscores.txt"

extern void Highscore_EnterState();
extern void Highscore_ExitState();
extern void Highscore_ProcessInput();
extern void Highscore_Update();
extern void Highscore_Render();