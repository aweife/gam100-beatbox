#include "Tutorial.h"
#include "../Global.h"
#include "../Map/Map.h"
#include "../Text/TextReader.h"
#include "../States/StateMachine.h"
#include "../States/Game.h"
#include "../Text/TextReader.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../Attack/Attack.h"
#include "../Clock/Clock.h"
#include "../Audio/AudioEngine.h"
#include "../Beat/Beat.h"
#include <Windows.h>
#include <stdbool.h>

#define TUTORIAL_TEXT_OFFSET 50
#define TUTORIAL_BEATMAN_OFFSET 40
#define TUTORIAL_DIALOGUE_OFFSET 70
#define TUTORIAL_MIRROR_OFFSET 145


//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************

// Intro1: Brief Description of Game
// Intro2: Main controls of the Game
// Dialogue1: Move Player Demo
// Dialogue2: Dash Attack Player Demo
// Dialogue3: Dodge Enemy Demo
// Dialogue4: Attack Enemy Demo (Not done)
// Dialogue5: End Tutorial
// Dialogue6: Move on to real game after all this!

sprite leftBeatmanState1;
sprite rightBeatmanState1;
sprite leftBeatmanState2;
sprite rightBeatmanState2;
sprite Dialogue1;
sprite Dialogue2;
sprite Dialogue3;
sprite Dialogue5;
sprite Dialogue6;

static int currentIntro = 0;
static int spaceDown = false;
static bool RETURN_DOWN = true;
static bool startTutorial = false;
static bool startGame = false;
static bool spawnEnemy = false;
static bool animateBeatman = false;
static double tutorialDuration = 0.0;
static double animateDuration = 0.0;

//*********************************************************************************
//									INPUT
//*********************************************************************************
void Tutorial_ProcessInput()
{
	if (GetAsyncKeyState(VK_RETURN) && !RETURN_DOWN && currentIntro == 0)
	{
		RETURN_DOWN = true;
		// Everything from here onwards will execute once
		currentIntro = 1;
	}
	/* else if (GetAsyncKeyState(VK_RETURN) && !RETURN_DOWN && startGame == true) {
		StateMachine_ChangeState(State_Game);
	}*/
	else if (!GetAsyncKeyState(VK_RETURN)) {
		RETURN_DOWN = false;
	}

	// Allows player to move
	/*if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_UP))
		Player_SetVel(TOPLEFT, SpeedUp);
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_UP))
		Player_SetVel(TOPRIGHT, SpeedUp);
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_DOWN))
		Player_SetVel(BOTTOMRIGHT, SpeedUp);
	else if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_DOWN))
		Player_SetVel(BOTTOMLEFT, SpeedUp);
	else if (GetAsyncKeyState(VK_UP))
		Player_SetVel(UP, SpeedUp);
	else if (GetAsyncKeyState(VK_RIGHT))
		Player_SetVel(RIGHT, SpeedUp);
	else if (GetAsyncKeyState(VK_DOWN))
		Player_SetVel(DOWN, SpeedUp);
	else if (GetAsyncKeyState(VK_LEFT))
		Player_SetVel(LEFT, SpeedUp);
	else
	{
		if (Player_GetDirection() != STAY)
		{
			Player_SetVel(Player_GetDirection(), SlowDown);
			if (Player_GetEaseFactor() <= 0)
				Player_SetVel(STAY, SlowDown);
		}
	}

	if (GetAsyncKeyState(VK_SPACE) && !spaceDown)
	{
		spaceDown = true;
		Player_Dash();
	}
	else if (!GetAsyncKeyState(VK_SPACE)) spaceDown = false;*/
}

//*********************************************************************************
//									UPDATE
//*********************************************************************************
void Tutorial_Update()
{
	Clock_GameLoopStart();
	Audio_Update();
	Beat_Update();
	Player_Update();

	if (currentIntro == 0)
	{
		animateDuration += Clock_GetDeltaTime();
	}

	/*if (currentIntro == 1)
	{
		tutorialDuration += Clock_GetDeltaTime();
	}*/
}

//*********************************************************************************
//									RENDER
//*********************************************************************************
void Tutorial_Render()
{
	if (currentIntro == 0 && animateDuration >= 0.0 && animateDuration <= 1000.0)
	{
		animateBeatman = false;
		Text_Render(&leftBeatmanState1, 0, 0);
		Text_Render(&rightBeatmanState1, 0, 0);
	}

	if (currentIntro == 0 && animateDuration >= 1000.0 && animateDuration <= 2000.0)
	{
		animateBeatman = true;
	}

	if (currentIntro == 0 && animateBeatman == true)
	{
		Text_Render(&leftBeatmanState2, 0, 0);
		Text_Render(&rightBeatmanState2, 0, 0);
	}

	if (currentIntro == 0 && animateDuration >= 2000.0)
	{
		animateDuration -= 2000.0;
	}

	/*if (currentIntro == 0 && animateDuration >= 1000.0 && animateDuration)*/

	if (currentIntro == 1)
	{
		Map_Render();
		Player_Render();

		// For 10 seconds
		if (tutorialDuration >= 0.0 && tutorialDuration <= 10000.0)
		{
			if (!startTutorial)
				Audio_PlayBGMWithDelay(0.001, TUTORIAL);
			startTutorial = true;
			Text_RenderWords(&Dialogue1, 0, 0);
		}
		// For 10 seconds
		else if (tutorialDuration > 10000.0 && tutorialDuration <= 20000.0) {
			Text_RenderWords(&Dialogue2, 0, 0);
		}
		else if (tutorialDuration > 20000.0 && tutorialDuration <= 30000.0) {
			Text_RenderWords(&Dialogue3, 0, 0);
			if (!spawnEnemy)
				Enemy_Init();
			spawnEnemy = true;
			Enemy_Render();
			Attack_Render();
		}
		else if (tutorialDuration > 30000.0) {
			if (!startGame)
				
			startGame = true;
			Text_RenderWords(&Dialogue5, 0, 0);
		}
	}
}

//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void Tutorial_EnterState()
{
	// Brief Description Of Game
	leftBeatmanState1 = Text_CreateSprite();
	Text_Init(&leftBeatmanState1, "..//RhythmGame//$Resources//beatman1.txt");
	Text_Move(&leftBeatmanState1, (GAME_WIDTH / 4) - TUTORIAL_BEATMAN_OFFSET, (GAME_HEIGHT / 4) - (TUTORIAL_BEATMAN_OFFSET / 2));

	rightBeatmanState1 = Text_CreateSprite();
	Text_Init(&rightBeatmanState1, "..//RhythmGame//$Resources//beatman2.txt");
	Text_Move(&rightBeatmanState1, (GAME_WIDTH / 4) - (TUTORIAL_BEATMAN_OFFSET - 2) + TUTORIAL_MIRROR_OFFSET, (GAME_HEIGHT / 4) - (TUTORIAL_BEATMAN_OFFSET / 2) - 5);

	leftBeatmanState2 = Text_CreateSprite();
	Text_Init(&leftBeatmanState2, "..//RhythmGame//$Resources//beatman1.txt");
	Text_Move(&leftBeatmanState2, (GAME_WIDTH / 4) - (TUTORIAL_BEATMAN_OFFSET - 2) + TUTORIAL_MIRROR_OFFSET, (GAME_HEIGHT / 4) - (TUTORIAL_BEATMAN_OFFSET / 2));

	rightBeatmanState2 = Text_CreateSprite();
	Text_Init(&rightBeatmanState2, "..//RhythmGame//$Resources//beatman2.txt");
	Text_Move(&rightBeatmanState2, (GAME_WIDTH / 4) - TUTORIAL_BEATMAN_OFFSET, (GAME_HEIGHT / 4) - (TUTORIAL_BEATMAN_OFFSET / 2) - 5);

	// Demo to Move Player
	Audio_Init();
	Map_Init();
	Beat_Init();
	Player_Init();
	Attack_Init();

	// Tutorial on moving player
	Dialogue1 = Text_CreateSprite();
	Text_Init(&Dialogue1, "..//RhythmGame//$Resources//DialogueMovePlayer.txt");
	Text_Move(&Dialogue1, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSET, (GAME_HEIGHT)-TUTORIAL_DIALOGUE_OFFSET / 1.5);

	// Tutorial on dashing attack
	Dialogue2 = Text_CreateSprite();
	Text_Init(&Dialogue2, "..//RhythmGame//$Resources//DialogueDashAttack.txt");
	Text_Move(&Dialogue2, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSET, (GAME_HEIGHT)-TUTORIAL_DIALOGUE_OFFSET / 1.5);

	// Tutorial on Dodging Enemy
	Dialogue3 = Text_CreateSprite();
	Text_Init(&Dialogue3, "..//RhythmGame//$Resources//DialogueDodgeEnemy.txt");
	Text_Move(&Dialogue3, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSET, (GAME_HEIGHT)-TUTORIAL_DIALOGUE_OFFSET / 1.5);

	// End Tutorial
	Dialogue5 = Text_CreateSprite();
	Text_Init(&Dialogue5, "..//RhythmGame//$Resources//DialogueEndTutorial.txt");
	Text_Move(&Dialogue5, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSET, (GAME_HEIGHT)-TUTORIAL_DIALOGUE_OFFSET / 1.5);
}

void Tutorial_ExitState()
{

}