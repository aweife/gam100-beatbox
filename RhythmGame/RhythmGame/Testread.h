
typedef struct
{
	int Xposition;
	int Yposition;
	int position[500][2];
	char printchar[500];

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

