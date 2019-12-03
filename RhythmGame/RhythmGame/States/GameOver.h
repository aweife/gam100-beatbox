#pragma once

typedef enum DisplayScore {
	SOLO,
	NOTSOLO,
}DisplayScore;

//Initialise/re-initialise all variables when entering tutorial state
extern void GameOver_EnterState();

//Clean up existing values when exiting tutorial
extern void GameOver_ExitState();

//Provide transition and input to tutorial state
extern void GameOver_ProcessInput();

//Allows update of variables and sprites in tutorial state
extern void GameOver_Update();

//Renders all sprites and animation in tutorial state
extern void GameOver_Render();

//Always set score after game
extern void GameOver_SetScore(DisplayScore mode, int score, int score2);