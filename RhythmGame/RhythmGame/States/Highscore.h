#pragma once

//keeps track of highscore sprite and whether it is visible
typedef struct Phighscore
{
	int visible;
	sprite printsprite;
}Phighscore;

//keeps track of name sprite and whether it is visible
typedef struct Pname
{
	int visible;
	sprite printsprite;
}Pname;

//keeps track of players with their name and highscore
typedef struct Pinfo
{
	Phighscore playerscore[8];
	Pname playername[3];
	int score;
}Pinfo;

#define HIGHSCORE_PATH "$HighScores//highscores.txt"

//statemachine functions
extern void Highscore_EnterState();
extern void Highscore_ExitState();
extern void Highscore_ProcessInput();
extern void Highscore_Update();
extern void Highscore_Render();

//Renders Animation for Trophyman
void _RenderTrophymanAnimation();