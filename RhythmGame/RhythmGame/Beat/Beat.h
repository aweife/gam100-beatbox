#pragma once

#include <stdbool.h>
#include "../Audio/AudioEngine.h"

extern void Beat_Init();
extern void Beat_Update();
extern bool Beat_GetBeat(TRACKTYPE type);