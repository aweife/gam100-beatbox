#pragma once

typedef enum DisplayScore {
	SOLO,
	NOTSOLO,
}DisplayScore;

extern void GameOver_EnterState();
extern void GameOver_ExitState();
extern void GameOver_ProcessInput();
extern void GameOver_Update();
extern void GameOver_Render();

// Always set score after game
extern void GameOver_SetScore(int score, DisplayScore mode, int score2);