#include "StateMachine.h"
#include "Game.h"
#include "MainMenu.h"
#include "GameOver.h"
#include "Logo.h"
#include "../Console/Console.h"
#include "../Tutorial/Tutorial.h"

static GameState currentState = State_Default;
static GameState reqState = State_Default;
static int gameMode = 0;

void StateMachine_Start()
{
	if (currentState != reqState)
	{
		switch (currentState)
		{
		case State_Logo: Logo_ExitState(); break;
		case State_MainMenu: MainMenu_ExitState(); break;
		//case State_Tutorial: Tutorial_ExitState(); break;
		case State_Game: Game_ExitState(); break;
		case State_GameOver: GameOver_ExitState(); break;
		}

		currentState = reqState;

		switch (currentState)
		{
		case State_Logo: Logo_EnterState(); break;
		case State_MainMenu: MainMenu_EnterState(); break;
		//case State_Tutorial: Tutorial_EnterState(); break;
		case State_Game: Game_EnterState(gameMode); break;
		case State_GameOver: GameOver_EnterState(); break;
		}
	}
}

void StateMachine_ChangeState(GameState newState)
{
	reqState = newState;
}

void StateMachine_ProcessInput()
{
	switch (currentState)
	{
	case State_Logo: Logo_ProcessInput(); break;
	case State_MainMenu: MainMenu_ProcessInput(); break;
	//case State_Tutorial: Tutorial_ProcessInput(); break;
	case State_Game: Game_ProcessInput(); break;
	case State_GameOver: GameOver_ProcessInput(); break;
	}
}

void StateMachine_Update()
{
	switch (currentState)
	{
	case State_Logo: Logo_Update(); break;
	case State_MainMenu: MainMenu_Update(); break;
	//case State_Tutorial: Tutorial_Update(); break;
	case State_Game: Game_Update(); break;
	case State_GameOver: GameOver_Update(); break;
	}
}

void StateMachine_Render()
{
	Console_ClearRenderBuffer();

	switch (currentState)
	{
	case State_Logo: Logo_Render(); break;
	case State_MainMenu: MainMenu_Render(); break;
	//case State_Tutorial: Tutorial_Render(); break;
	case State_Game: Game_Render(); break;
	case State_GameOver: GameOver_Render(); break;
	}

	Console_SwapRenderBuffer();
}

void StateMachine_ChangeMode(int newMode)
{
	gameMode = newMode;
}