#pragma once

#define ENEMY_SIZE 500


typedef struct
{
	int Xposition;
	int Yposition;
	int position[ENEMY_SIZE][2];
	char printchar[ENEMY_SIZE];

}skullenemy;

//initialise for skullenemy
void Text_Init();

//creates a skullenemy
skullenemy Text_CreateEnemy();

//Renders enemy
void Text_RenderEnemy(skullenemy* s);

// Set enemy position
void Text_Moveenemy(skullenemy* s, int x, int y);

//Read from text file and store in array
void _Readandstoretext();

