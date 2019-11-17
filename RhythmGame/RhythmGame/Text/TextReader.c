#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include "TextReader.h"
#include "../Console/Console.h"
#include "../Global.h"

FILE* pFile;
sprite skullenemy = { 0 };
char Charline[150] = { 0 };
int length = 0;
int charcount = 0;
int newcharcount = 0;
int totalcharcount = 0;
int currentyposition = 0;
int iteration = 0;

void Text_Init(sprite *s, char* path)
{   
	_Readandstoretext(s,path);
}

sprite Text_CreateSprite()
{
	return skullenemy;
}
void Text_Render(sprite *s)
{
	for (int i = 0; i < SPRITE_SIZE; i++)
	{
		if (s->printchar[i] == 'R')
	      Console_SetRenderBuffer_CharColor((s->position[i][0]) + s->Xposition, (s->position[i][1] + s->Yposition),s->printchar[i], bRED);
		else if(s->printchar[i] == 'W')
		  Console_SetRenderBuffer_CharColor((s->position[i][0]) + s->Xposition, (s->position[i][1] + s->Yposition),s->printchar[i], WHITE);
		else
		Console_SetRenderBuffer_Char((s->position[i][0])+s->Xposition,(s->position[i][1]+s->Yposition),(s->printchar[i]));
    }
}

void Text_Move(sprite *skull,int x, int y)
{
	skull->Xposition = x;
	skull->Yposition = y;
}

void _Readandstoretext(sprite *s, char* path)
{
	iteration = 0;
	newcharcount = 0;
	totalcharcount = 0;

	for (int i = 0; i < 150; i++)
	{
		Charline[i] = '0';
	}

	pFile = fopen(path, "r");
	if (pFile == NULL) perror("Error opening file");
	else
	{
		while (!feof(pFile))
		{
			charcount = 0;

			fgets(Charline, 150, pFile);


			for (int i = 0; i < 150; i++)
			{
				if (Charline[i] != '0' && Charline[i] != '\n' && Charline[i] != '\0')
				{

					charcount++;
				}
				else
				{
					Charline[i] = '0';
				}


			}

			printf("%d\n", charcount);
			
			for (int i = 0; i < charcount; i++)
			{
				if (Charline[i] != ' ')
				{
					s->position[newcharcount + iteration][0] = i;
					s->position[newcharcount + iteration][1] = currentyposition;
					s->printchar[newcharcount + iteration] = Charline[i];
					iteration++;
				}
			}

			totalcharcount = newcharcount + iteration;

			newcharcount = totalcharcount;
			currentyposition++;
		}
		fclose(pFile);
	}
}
