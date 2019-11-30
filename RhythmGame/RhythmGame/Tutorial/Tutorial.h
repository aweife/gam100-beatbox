#pragma once

extern void Tutorial_EnterState();
extern void Tutorial_ExitState();
extern void Tutorial_ProcessInput();
extern void Tutorial_Update();
extern void Tutorial_Render();
extern void Tutorial_ExitState();

//Contains all the sprites in Instruction Page
void InstructionSprite_Init();

//Contains all the sprites in the Gameplay Page
void GameplaySprite_Init();

//Renders the Animation of Beatman
void _RenderBeatmanAnimation();

//Renders the Animaton of the BeatHead
void _RenderBeatHeadAnimation();

//Renders the Animation of the Enter Key
void _RenderEnterAnimation();

//Renders the Animation of the Arrow Keys
void _RenderArrowKeysAnimation();

//Renders the Animation of the Space Key
void _RenderSpaceKeyAnimation();

//Renders the Anumation of Enemy Warning
void _RenderWarningAnimation();

//Enable Player Movement
static void _MovePlayer();