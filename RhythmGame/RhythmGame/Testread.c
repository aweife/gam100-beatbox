#define _CRT_SECURE_NO_DEPRECATE
#include<stdlib.h>
#include <stdio.h>
#include"Console/Console.h"
#include"Testread.h"

FILE* pFile;
skullenemy s;
char Charline[150] = { 0 };
int length = 0;
int charcount = 0;
int newcharcount = 0;
int totalcharcount = 0;
int currentyposition = 0;
int iteration = 0;

void Text_Init()
{
	_Readandstoretext();
}

skullenemy Text_CreateEnemy()
{
	return s;
}

void Text_RenderEnemy(skullenemy *s)
{
	for (int i = 0; i < 500; i++)
	{
		
		Console_SetRenderBuffer_Char((s->position[i][0])+s->Xposition,(s->position[i][1]+s->Yposition),(s->printchar[i]));
    }
}

void Text_Moveenemy(skullenemy *skull,int x, int y)
{
	skull->Xposition = x;
	skull->Yposition = y;
}

void _Readandstoretext()
{
	pFile = fopen("skull.txt", "r");
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

			totalcharcount = newcharcount + charcount;

			for (int i = 0; i < charcount; i++)
			{

				s.position[newcharcount + i][0] = i;
				s.position[newcharcount + i][1] = currentyposition;
				s.printchar[newcharcount + i] = Charline[i];

			}
			newcharcount = totalcharcount;
			currentyposition++;
			iteration++;

		}
		fclose(pFile);
	}
}
