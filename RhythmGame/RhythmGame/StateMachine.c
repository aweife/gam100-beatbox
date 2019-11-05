#include "StateMachine.h"
#include "Console/Console.h"
#include "Game.h"

static GameState currentState = State_Default;
static GameState reqState = State_Default;

void StateMachine_Start()
{
	if (currentState != reqState)
	{
		switch (currentState)
		{
		case State_Game: Game_ExitState(); break;
		}
	}

	currentState = reqState;

	switch (currentState)
	{
	case State_Game: Game_EnterState(); break;
	}
}

void StateMachine_changeState(GameState newState)
{
	reqState = newState;
}

void StateMachine_ProcessInput()
{
	switch (currentState)
	{
	case State_Game: Game_ProcessInput(); break;
	}
}

void StateMachine_Update()
{
	switch (currentState)
	{
	case State_Game: Game_Update(); break;
	}
}

void StateMachine_Render()
{
	Console_ClearRenderBuffer();

	switch (currentState)
	{
	case State_Game: Game_Render(); break;
	}

	Console_SwapRenderBuffer();
}