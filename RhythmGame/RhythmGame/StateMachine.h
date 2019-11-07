#ifndef GAMESTATE_H
#define GAMESTATE_H

typedef enum GameState
{
	State_Default,
	State_MainMenu,
	State_Game,
	State_GameOver,
	State_Pause,
}GameState;

void StateMachine_Start();
void StateMachine_changeState(GameState newstate);
void StateMachine_ProcessInput();
void StateMachine_Update();
void StateMachine_Render();

#endif