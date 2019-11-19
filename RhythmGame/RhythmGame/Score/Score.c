#define _CRT_SECURE_NO_DEPRECATE

#include "Score.h"

FILE* writeScoreFile;
FILE* readScoreFile;

static char scoreDisplay[256];
static char buffer[BUFFER_SIZE];

Player temp;
Player p[SIZE];

//Internal Functions

void _RenderScoreBoard(int score, int x, int y)
{
	_itoa_s(score, scoreDisplay, sizeof(scoreDisplay), 10);
	Console_SetRenderBuffer_String(x, y, "Score:");
	Console_SetRenderBuffer_String(x + 7, y, scoreDisplay);
}

void _InputScore(char* path)
{
	//Write To Score File
	writeScoreFile = fopen(path, "a");
	if (writeScoreFile == NULL)
	{
		perror("Error opening file");
	}
	else {
		fprintf(writeScoreFile, "Player Score: %d\n", Get_Score());
		fclose(writeScoreFile);
	}
}

void _OutputScore(char* path, int x, int y)
{
	//Read from Score File
	readScoreFile = fopen(path, "rt");
	if (readScoreFile == NULL)
	{
		perror("Error opening file");
	}
	else {
		while (!feof(writeScoreFile))
		{
			if (fgets(buffer, BUFFER_SIZE, writeScoreFile))
			{

				for (int i = 0; i < BUFFER_SIZE; i++)
				{
					if (buffer[i] == '\n')
						buffer[i] = ' ';

				}

				Console_SetRenderBuffer_String(x, y, buffer);
				y += 1;
				//printf(buffer);
			}

		}
		fclose(writeScoreFile);
	}
}