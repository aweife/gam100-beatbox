#include "Font.h"

// Word sprites
typedef struct Words {
	sprite atoj[10];
	sprite ktot[10];
	sprite utoz[6];
	sprite numbers[10];
} Words;
Words words;

// Internals
void _InitLettersAndNumbers();

void Font_Create()
{
	_InitLettersAndNumbers();
}

void Font_Cleanup()
{
	// Destroy a to j
	for (int i = 0; i < 10; i++)
		Text_Cleanup(&words.atoj[i]);

	// Destroy k to t
	for (int i = 0; i < 10; i++)
		Text_Cleanup(&words.ktot[i]);

	// Destroy u to z
	for (int i = 0; i < 6; i++)
		Text_Cleanup(&words.utoz[i]);

	// Destroy numbers
	for (int i = 0; i < 10; i++)
		Text_Cleanup(&words.numbers[i]);
}

void _InitLettersAndNumbers()
{
	// Create a to j
	for (int i = 0; i < 10; i++)
	{
		words.atoj[i] = Text_CreateSprite();
		Text_InitArray(&words.atoj[i], "..//RhythmGame//$Resources//Alphabets.txt", i);
	}
	// Create k to t
	for (int i = 0; i < 10; i++)
	{
		words.ktot[i] = Text_CreateSprite();
		Text_InitArray(&words.ktot[i], "..//RhythmGame//$Resources//Alphabets2.txt", i);
	}
	// Create u to z
	for (int i = 0; i < 6; i++)
	{
		words.utoz[i] = Text_CreateSprite();
		Text_InitArray(&words.utoz[i], "..//RhythmGame//$Resources//Alphabets3.txt", i);
	}
	// Create numbers
	for (int i = 0; i < 10; i++)
	{
		words.numbers[i] = Text_CreateSprite();
		Text_InitArray(&words.numbers[i], "..//RhythmGame//$Resources//numbers.txt", i);
	}
}

sprite Font_ConvertToSprite(int number)
{
	return words.atoj[number];
}