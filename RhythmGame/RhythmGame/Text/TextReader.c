#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include "TextReader.h"
#include "../Console/Console.h"
#include "../Global.h"


FILE *pFile;
sprite skullenemy;
char Charline[150] = { 0 };
int length = 0;
int charcount = 0;
int newcharcount = 0;
int totalcharcount = 0;
int currentyposition = 0;
int iteration = 0;
int Xoffset = 0;
int count = 0;

void Text_Init(sprite *s, char *path)
{
	 count = _CountChars(path);
	s->charCount = count;
	s->spriteI = (spriteInfo *)malloc(sizeof(spriteInfo) * count);
	_Readandstoretext(s, path);
}

void Text_Cleanup(sprite *s)
{
	free(s->spriteI);
}

int _CountChars(char * path)
{
	totalcharcount = 0;

	pFile = fopen(path, "r");
	if (pFile == NULL)
	{
		perror("Error opening file");
		return 0;
	}
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

			totalcharcount +=charcount;
		}

		fclose(pFile);
		return totalcharcount;
	}

}


void Text_InitArray(sprite *s, char *path)
{
	_ReadandstoretextArray(s, path);

}

sprite Text_CreateSprite()
{
	return skullenemy;
}
void Text_Render(sprite *s, int offsetX, int offsetY)
{
	for (int i = 0; i < s->charCount; i++)
	{
		Console_SetRenderBuffer_CharColor((s->spriteI[i].position.x + s->origin.x) + offsetX, (s->spriteI[i].position.y + s->origin.y) + offsetY, ' ', (WORD)s->spriteI[i].printColor);
		if (s->spriteI[i].printchar == '\0')
			break;
	}
	Console_SetRenderBuffer_CharColor((s->spriteI[0].position.x) + s->origin.x, (s->spriteI[0].position.y + s->origin.y), ' ', (WORD)s->spriteI[0].printColor);
}



void Text_RenderRainbow(sprite *s)
{
	int rainbowinterator = 1;
	int oldyvalue = 0;
	CONSOLECOLOR rainbow;

	for (int i = 0; i < s->charCount; i++)
	{
		if (s->spriteI[i].position.y > oldyvalue)
		{
			rainbowinterator++;
			if (rainbowinterator == 7)
				rainbowinterator = 1;

		}

		oldyvalue = s->spriteI[i].position.y;


		switch (rainbowinterator)
		{
		case 1: rainbow = RED;
			break;
		case 2: rainbow = YELLOW;
			break;
		case 3: rainbow = GREEN;
			break;
		case 4: rainbow = CYAN;
			break;
		case 5: rainbow = DARKCYAN;
			break;
		case 6: rainbow = MAGENTA;
			break;
		}
		Console_SetRenderBuffer_CharColor((s->spriteI[i].position.x) + s->origin.x, (s->spriteI[i].position.y + s->origin.y), ' ', rainbow);

		if (s->spriteI[i].printchar == '\0')
			break;
	}
}

void Text_Move(sprite *s, int x, int y)
{
	s->origin.x = x;
	s->origin.y = y;
}

void _Readandstoretext(sprite *s, const char *path)
{
	iteration = 0;
	newcharcount = 0;
	totalcharcount = 0;
	Xoffset = 0;
	currentyposition = 0;

	for (int i = 0; i < 150; i++)
	{
		Charline[i] = '0';
	}

	pFile = fopen(path, "r");
	if (pFile == NULL)
		perror("Error opening file");
	else
	{
		while (!feof(pFile))
		{
			charcount = 0;
			Xoffset = 0;
			iteration = 0;

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

			for (int i = 0; i < charcount; i++)
			{
				if (Charline[i] != ' ' && Charline[i] != '0')
				{
					s->spriteI[newcharcount + iteration].position.x = Xoffset;
					s->spriteI[newcharcount + iteration].position.y = currentyposition;
					s->spriteI[newcharcount + iteration].printchar = Charline[i];
					if (Charline[i] == 'g')
						s->spriteI[newcharcount + iteration].printColor = GREEN;
					else if (Charline[i] == 'G')
						s->spriteI[newcharcount + iteration].printColor = DARKGREEN;
					else if (Charline[i] == 'b')
						s->spriteI[newcharcount + iteration].printColor = BLUE;
					else if (Charline[i] == 'B')
						s->spriteI[newcharcount + iteration].printColor = DARKBLUE;
					else if (Charline[i] == 'c')
						s->spriteI[newcharcount + iteration].printColor = CYAN;
					else if (Charline[i] == 'C')
						s->spriteI[newcharcount + iteration].printColor = DARKCYAN;
					else if (Charline[i] == 'y')
						s->spriteI[newcharcount + iteration].printColor = YELLOW;
					else if (Charline[i] == 'Y')
						s->spriteI[newcharcount + iteration].printColor = DARKYELLOW;
					else if (Charline[i] == 'r')
						s->spriteI[newcharcount + iteration].printColor = RED;
					else if (Charline[i] == 'R')
						s->spriteI[newcharcount + iteration].printColor = DARKRED;
					else if (Charline[i] == 'm')
						s->spriteI[newcharcount + iteration].printColor = MAGENTA;
					else if (Charline[i] == 'M')
						s->spriteI[newcharcount + iteration].printColor = DARKMAGENTA;
					else if (Charline[i] == 'w')
						s->spriteI[newcharcount + iteration].printColor = WHITE;
					else if (Charline[i] == 'W')
						s->spriteI[newcharcount + iteration].printColor = BLACK;
					else if (Charline[i] == 'd')
						s->spriteI[newcharcount + iteration].printColor = DARKGRAY;
					else if (Charline[i] == 'D')
						s->spriteI[newcharcount + iteration].printColor = GRAY;
					else if (Charline[i] == 'B')
						s->spriteI[newcharcount + iteration].printColor = DARKBLUE;
					else
						s->spriteI[newcharcount + iteration].printColor = WHITE;

					iteration++;
				}

				Xoffset++;
			}

			totalcharcount = newcharcount + iteration;
			s->charCount = totalcharcount;
			newcharcount = totalcharcount;
			currentyposition++;
		}
		fclose(pFile);
	}
}

void Text_RenderWords(sprite* s)
{
	for (int i = 0; i < s->charCount; i++)
	{
		Console_SetRenderBuffer_CharColor((s->spriteI[i].position.x) + s->origin.x, (s->spriteI[i].position.y + s->origin.y), s->spriteI[i].printchar, fWHITE);
		if (s->spriteI[i].printchar == '\0')
			break;
	}
	Console_SetRenderBuffer_CharColor((s->spriteI[0].position.x) + s->origin.x, (s->spriteI[0].position.y + s->origin.y), s->spriteI[0].printchar, WHITE);
}

void _ReadandstoretextArray(sprite *s, const char *path)
{
	int state = 0;
	iteration = 0;
	newcharcount = 0;
	totalcharcount = 0;
	Xoffset = 0;
	currentyposition = 0;

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
			Xoffset = 0;
			iteration = 0;

			fgets(Charline, 150, pFile);

			if (isdigit(Charline[0]))
			{
				state = Charline[0] - '0';
				newcharcount = 0;
				totalcharcount = 0;
				iteration = 0;
				currentyposition = 0;
				continue;
			}


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
				if (Charline[i] != ' ' && Charline[i] != '\0')
				{
					(s + state)->spriteI[newcharcount + iteration].position.x = Xoffset;
					(s + state)->spriteI[newcharcount + iteration].position.y = currentyposition;
					(s + state)->spriteI[newcharcount + iteration].printchar= Charline[i];
					if (Charline[i] == 'g')
						(s + state)->spriteI[newcharcount + iteration].printColor = GREEN;
					else if (Charline[i] == 'G')
						(s + state)->spriteI[newcharcount + iteration].printColor = DARKGREEN;
					else if (Charline[i] == 'b')
						(s + state)->spriteI[newcharcount + iteration].printColor = BLUE;
					else if (Charline[i] == 'B')
						(s + state)->spriteI[newcharcount + iteration].printColor = DARKBLUE;
					else if (Charline[i] == 'c')
						(s + state)->spriteI[newcharcount + iteration].printColor = CYAN;
					else if (Charline[i] == 'C')
						(s + state)->spriteI[newcharcount + iteration].printColor = DARKCYAN;
					else if (Charline[i] == 'y')
						(s + state)->spriteI[newcharcount + iteration].printColor = YELLOW;
					else if (Charline[i] == 'Y')
						(s + state)->spriteI[newcharcount + iteration].printColor = DARKYELLOW;
					else if (Charline[i] == 'r')
						(s + state)->spriteI[newcharcount + iteration].printColor = RED;
					else if (Charline[i] == 'R')
						(s + state)->spriteI[newcharcount + iteration].printColor = DARKRED;
					else if (Charline[i] == 'm')
						(s + state)->spriteI[newcharcount + iteration].printColor = MAGENTA;
					else if (Charline[i] == 'M')
						(s + state)->spriteI[newcharcount + iteration].printColor = DARKMAGENTA;
					else if (Charline[i] == 'w')
						(s + state)->spriteI[newcharcount + iteration].printColor = WHITE;
					else if (Charline[i] == 'W')
						(s + state)->spriteI[newcharcount + iteration].printColor = BLACK;
					else if (Charline[i] == 'd')
						(s + state)->spriteI[newcharcount + iteration].printColor = GRAY;
					else if (Charline[i] == 'D')
						(s + state)->spriteI[newcharcount + iteration].printColor = DARKGRAY;
					else if (Charline[i] == 'B')
						(s + state)->spriteI[newcharcount + iteration].printColor = DARKBLUE;
					else
						(s + state)->spriteI[newcharcount + iteration].printColor = WHITE;

					iteration++;
				}

				Xoffset++;
			}

			totalcharcount = newcharcount + iteration;

			newcharcount = totalcharcount;
			currentyposition++;
		}
		fclose(pFile);
	}
}