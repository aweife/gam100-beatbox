#pragma once

extern void Tutorial_EnterState();
extern void Tutorial_ExitState();
extern void Tutorial_ProcessInput();
extern void Tutorial_Update();
extern void Tutorial_Render();
extern void Tutorial_ExitState();

void InstructionSprite_Init();
void GameplaySprite_Init();
void _RenderBeatmanAnimation();
void _RenderBeatHeadAnimation();
void _RenderEnterAnimation();
void _RenderArrowKeysAnimation();
void _RenderSpaceKeyAnimation();
static void _MovePlayer();