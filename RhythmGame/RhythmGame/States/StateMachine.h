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

// 
extern void StateMachine_Start();

// Changes state between statemachine
extern void StateMachine_ChangeState(GameState newstate);

// Runs specific process input depending on state
extern void StateMachine_ProcessInput();

// Runs specific update depending on state
extern void StateMachine_Update();

// Renders depending on state
extern void StateMachine_Render();

// Changes game mode (ONEPLAYER, TWOPLAYER)
extern void StateMachine_ChangeMode(int newMode);
