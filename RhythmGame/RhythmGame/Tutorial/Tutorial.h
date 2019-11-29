#pragma once

extern void Tutorial_EnterState();
extern void Tutorial_ExitState();
extern void Tutorial_ProcessInput();
extern void Tutorial_Update();
extern void Tutorial_Render();
extern void Tutorial_ExitState();
void TutorialSprite_Init();
void _RenderBeatmanAnimation();
void _RenderEnterAnimation();
static void _MovePlayer();