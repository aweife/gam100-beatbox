#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <ctype.h>
#include "TextReader.h"
#include "../Console/Console.h"
#include "../Global.h"


FILE *pFile;
sprite skullenemy = { 0 };
char Charline[150] = { 0 };
int length = 0;
int charcount = 0;
int newcharcount = 0;
int totalcharcount = 0;
int currentyposition = 0;
int iteration = 0;
int Xoffset = 0;


void Text_Init(sprite *s, char *path)
{
	_Readandstoretext(s, path);
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
	for (int i = 0; i < SPRITE_SIZE; i++)
	{
		Console_SetRenderBuffer_CharColor((s->position[i].x + s->origin.x) + offsetX, (s->position[i].y + s->origin.y) + offsetY, ' ', (WORD)s->printColor[i]);
		if (s->printchar[i] == '\0')
			break;
	}
	Console_SetRenderBuffer_CharColor((s->position[0].x) + s->origin.x, (s->position[0].y + s->origin.y), ' ', (WORD)s->printColor[0]);
}



void Text_RenderRainbow(sprite *s)
{
	int rainbowinterator = 1;
	int oldyvalue = 0;
	CONSOLECOLOR rainbow;

	for (int i = 0; i < SPRITE_SIZE; i++)
	{
		if (s->position[i].y > oldyvalue)
		{
			rainbowinterator++;
			if (rainbowinterator == 7)
				rainbowinterator = 1;

		}

		oldyvalue = s->position[i].y;


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
		Console_SetRenderBuffer_CharColor((s->position[i].x) + s->origin.x, (s->position[i].y + s->origin.y), ' ', rainbow);

		if (s->printchar[i] == '\0')
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
	if (pFile == NULL) perror("Error opening file");
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

			printf("%d\n", charcount);

			for (int i = 0; i < charcount; i++)
			{
				if (Charline[i] != ' ' && Charline[i] != '0')
				{
					s->position[newcharcount + iteration].x = Xoffset;
					s->position[newcharcount + iteration].y = currentyposition;
					s->printchar[newcharcount + iteration] = Charline[i];
					if (Charline[i] == 'g')
						s->printColor[newcharcount + iteration] = GREEN;
					else if (Charline[i] == 'G')
						s->printColor[newcharcount + iteration] = DARKGREEN;
					else if (Charline[i] == 'b')
						s->printColor[newcharcount + iteration] = BLUE;
					else if (Charline[i] == 'B')
						s->printColor[newcharcount + iteration] = DARKBLUE;
					else if (Charline[i] == 'c')
						s->printColor[newcharcount + iteration] = CYAN;
					else if (Charline[i] == 'C')
						s->printColor[newcharcount + iteration] = DARKCYAN;
					else if (Charline[i] == 'y')
						s->printColor[newcharcount + iteration] = YELLOW;
					else if (Charline[i] == 'Y')
						s->printColor[newcharcount + iteration] = DARKYELLOW;
					else if (Charline[i] == 'r')
						s->printColor[newcharcount + iteration] = RED;
					else if (Charline[i] == 'R')
						s->printColor[newcharcount + iteration] = bRED;
					else if (Charline[i] == 'm')
						s->printColor[newcharcount + iteration] = MAGENTA;
					else if (Charline[i] == 'M')
						s->printColor[newcharcount + iteration] = DARKMAGENTA;
					else if (Charline[i] == 'w')
						s->printColor[newcharcount + iteration] = WHITE;
					else if (Charline[i] == 'W')
						s->printColor[newcharcount + iteration] = BLACK;
					else if (Charline[i] == 'd')
						s->printColor[newcharcount + iteration] = DARKGRAY;
					else if (Charline[i] == 'D')
						s->printColor[newcharcount + iteration] = GRAY;
					else if (Charline[i] == 'B')
						s->printColor[newcharcount + iteration] = DARKBLUE;
					else
						s->printColor[newcharcount + iteration] = WHITE;

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
					(s + state)->position[newcharcount + iteration].x = Xoffset;
					(s + state)->position[newcharcount + iteration].y = currentyposition;
					(s + state)->printchar[newcharcount + iteration] = Charline[i];
					if (Charline[i] == 'g')
						(s + state)->printColor[newcharcount + iteration] = GREEN;
					else if (Charline[i] == 'G')
						(s + state)->printColor[newcharcount + iteration] = DARKGREEN;
					else if (Charline[i] == 'b')
						(s + state)->printColor[newcharcount + iteration] = BLUE;
					else if (Charline[i] == 'B')
						(s + state)->printColor[newcharcount + iteration] = DARKBLUE;
					else if (Charline[i] == 'c')
						(s + state)->printColor[newcharcount + iteration] = CYAN;
					else if (Charline[i] == 'C')
						(s + state)->printColor[newcharcount + iteration] = DARKCYAN;
					else if (Charline[i] == 'y')
						(s + state)->printColor[newcharcount + iteration] = YELLOW;
					else if (Charline[i] == 'Y')
						(s + state)->printColor[newcharcount + iteration] = DARKYELLOW;
					else if (Charline[i] == 'r')
						(s + state)->printColor[newcharcount + iteration] = RED;
					else if (Charline[i] == 'R')
						(s + state)->printColor[newcharcount + iteration] = bRED;
					else if (Charline[i] == 'm')
						(s + state)->printColor[newcharcount + iteration] = MAGENTA;
					else if (Charline[i] == 'M')
						(s + state)->printColor[newcharcount + iteration] = DARKMAGENTA;
					else if (Charline[i] == 'w')
						(s + state)->printColor[newcharcount + iteration] = WHITE;
					else if (Charline[i] == 'W')
						(s + state)->printColor[newcharcount + iteration] = BLACK;
					else if (Charline[i] == 'd')
						(s + state)->printColor[newcharcount + iteration] = GRAY;
					else if (Charline[i] == 'D')
						(s + state)->printColor[newcharcount + iteration] = DARKGRAY;
					else if (Charline[i] == 'B')
						(s + state)->printColor[newcharcount + iteration] = DARKBLUE;
					else
						(s + state)->printColor[newcharcount + iteration] = WHITE;

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