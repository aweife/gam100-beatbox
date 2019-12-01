#pragma once

typedef enum GameState
{
	State_Default,
	State_Logo,
	State_MainMenu,
	State_Tutorial,
	State_Game,
	State_GameOver,
	State_Pause,
	State_Highscore,
}GameState;

extern void StateMachine_Start();
extern void StateMachine_ChangeState(GameState newstate);
extern void StateMachine_ProcessInput();
extern void StateMachine_Update();
extern void StateMachine_Render();
extern void StateMachine_ChangeMode(int newMode);
