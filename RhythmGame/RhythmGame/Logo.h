#pragma once
#include "Global.h"
#include "Text/TextReader.h"

typedef struct LogoStruct {
	Vector2d position;
	sprite LogoSprite;
} LogoStruct;

extern void Logo_EnterState();
extern void Logo_ExitState();
extern void Logo_ProcessInput();
extern void Logo_Update();
extern void Logo_Render();