#define _CRT_SECURE_NO_DEPRECATE

#include"Testread.h"

FILE* pFile;
skullenemy s = { 0 };
skullenemy logo = { 0 };
char Charline[150] = { 0 };
int length = 0;
int charcount = 0;
int newcharcount = 0;
int totalcharcount = 0;
int currentyposition = 0;
int iteration = 0;

void Text_Init()
{
	_Readandstoretext(&s,"skull.txt");
	_Readandstoretext(&logo,"Logo2.txt");
}

skullenemy Text_CreateEnemy()
{
	return s;
}

skullenemy Text_CreateLogo()
{
	return logo;
}

void Text_RenderEnemy(skullenemy *s)
{
	for (int i = 0; i < ENEMY_SIZE; i++)
	{
		
		Console_SetRenderBuffer_Char((s->position[i][0])+s->Xposition,(s->position[i][1]+s->Yposition),(s->printchar[i]));
    }
}

void Text_Moveenemy(skullenemy *skull,int x, int y)
{
	skull->Xposition = x;
	skull->Yposition = y;
}

void _Readandstoretext(skullenemy *s, char* path)
{

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
