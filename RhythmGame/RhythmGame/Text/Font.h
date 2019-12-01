#pragma once

#include "TextReader.h"

#define FONT_SPACING 6

// Create fonts to use, call once in main
extern void Font_Create();

extern void Font_Cleanup();

// Given 0 - 35, will return you the approriate sprite
extern sprite Font_ConvertToSprite(int number);