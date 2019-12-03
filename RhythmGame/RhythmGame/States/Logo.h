#pragma once
#include "../Global.h"
#include "../Text/TextReader.h"

typedef struct LogoStruct {
	Vector2d position;
	sprite LogoSprite1;
	sprite LogoSprite2;
} LogoStruct;


// Logo Enter State
extern void Logo_EnterState();

// Logo Exit State
extern void Logo_ExitState();

// Logo User Input
extern void Logo_ProcessInput();

// Logo Update
extern void Logo_Update();

// Logo Render
extern void Logo_Render();