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
#include <Windows.h>
#include <stdbool.h>

// For Instruction Page
#define TUTORIAL_TEXT_OFFSET 50
#define TUTORIAL_BEATMAN_OFFSET 40
#define TUTORIAL_INSTRUCTION_OFFSET 90
#define TUTORIAL_ENTER_OFFSET 35
#define TUTORIAL_MIRROR_OFFSET 145

// For Gameplay Page
#define TUTORIAL_DIALOGUE_OFFSETX 70
#define TUTORIAL_DIALOGUE_OFFSETY 45
//ENEMYX - 30 && ENEMY Y - 5
#define TUTORIAL_ENEMY_POSITIONX 60
#define TUTORIAL_ENEMY_POSITIONY 35


//*********************************************************************************
//								LOCAL VARIABLES
//*********************************************************************************

//Sprites of Pixel Art
sprite Instruction;
//Dialogue Entries
sprite MoveDialogue;
sprite DashDialogue;
sprite EnemyDialogue;
sprite NotesDialogue;
sprite EndTutorialDialogue;

//Sprites with Pixel Animation
//Beatman
sprite leftBeatmanState1;
sprite rightBeatmanState1;
sprite leftBeatmanState2;
sprite rightBeatmanState2;

//Enter Key
sprite EnterState1;
sprite EnterState2;

//BeatHead
sprite BeatheadState1;
sprite BeatheadState2;

//Arrow Keys
sprite ArrowKeysState1;
sprite ArrowKeysState2;

//Space Key
sprite SpaceKeyState1;
sprite SpaceKeyState2;

//Enemy Warning
sprite EnemyWarningState1;
sprite EnemyWarningState2;

//Controls State of Tutorial
static int currentIntro = 0;

//Boolean Variables
static bool spaceDown = false;
static bool RETURN_DOWN = true;
static bool startTutorial = false;
static bool startGame = false;
static bool spawnEnemy = false;
static bool animateBeatman = false;
static bool animateEnter = false;
static bool animateBeatHead = false;
static bool animateArrowKeys = false;
static bool animateSpaceKey = false;
static bool animateWarning = false;

//Controls Duration of States
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
	else if (GetAsyncKeyState(VK_RETURN) && !RETURN_DOWN && startGame == true) {
		StateMachine_ChangeState(State_Game);
	}
	else if (!GetAsyncKeyState(VK_RETURN)) {
		RETURN_DOWN = false;
	}

	if (currentIntro == 1)
	{
		_MovePlayer();
	}
}

//*********************************************************************************
//									UPDATE
//*********************************************************************************
void Tutorial_Update()
{
	Audio_Update();

	if (currentIntro == 1)
	{
		tutorialDuration += Clock_GetDeltaTime();
		Player_Update();
		Attack_Update();
		Enemy_Update();
	}
}

//*********************************************************************************
//									RENDER
//*********************************************************************************
void Tutorial_Render()
{
	if (currentIntro == 0)
	{
		Text_RenderRainbow(&Instruction, 0, 0);
		_RenderBeatmanAnimation();
		_RenderEnterAnimation();
	}

	if (currentIntro == 1)
	{
		Map_Render();
		Player_Render();
		_RenderBeatHeadAnimation();

		// Moves on after 10 seconds
		if (tutorialDuration >= 0.0 && tutorialDuration <= 10000.0)
		{
			if (!startTutorial)
			{
				Audio_Unload();
				Audio_Load(TUTORIAL);
				Audio_PlayBGM(TUTORIAL);
				startTutorial = true;
			}
			Text_Render(&MoveDialogue, 0, 0);
			_RenderArrowKeysAnimation();
		}
		// Moves on after 10 seconds
		else if (tutorialDuration > 10000.0 && tutorialDuration <= 20000.0) {
			Text_Render(&DashDialogue, 0, 0);
			_RenderSpaceKeyAnimation();
		}
		// Moves on after 20 seconds
		else if (tutorialDuration > 20000.0 && tutorialDuration <= 40000.0) {

			if (!spawnEnemy)
			{
				Enemy_Init();
				spawnEnemy = true;
			}

			if (tutorialDuration > 20000.0 && tutorialDuration <= 30000.0)
			{
				Text_Render(&EnemyDialogue, 0, 0);
				_RenderWarningAnimation();
			}

			if (tutorialDuration > 30000.0 && tutorialDuration <= 40000.0)
			{
				Text_Render(&NotesDialogue, 0, 0);
			}

			Enemy_Render();
			Attack_Render();
		}
		// Moves on after 10 seconds
		else if (tutorialDuration > 40000.0) {
			if (!startGame)	
			startGame = true;
			Text_Render(&EndTutorialDialogue, 0, 0);
		}
	}
}

//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void Tutorial_EnterState()
{
	Audio_Load(TUTORIAL);
	Audio_PlayBGM(TUTORIAL);
	InstructionSprite_Init();
	GameplaySprite_Init();
	Map_Init();
	Player_Init();
	Enemy_Init();
	Attack_Init();
}

void Tutorial_ExitState()
{
	Audio_Unload();
	Attack_Cleanup();
}

//*********************************************************************************
//						      INTERNAL FUNCTIONS
//*********************************************************************************

void InstructionSprite_Init()
{
	leftBeatmanState1 = Text_CreateSprite();
	Text_Init(&leftBeatmanState1, "..//RhythmGame//$Resources//beatman1.txt");
	Text_Move(&leftBeatmanState1, (GAME_WIDTH / 4) - TUTORIAL_BEATMAN_OFFSET, (GAME_HEIGHT / 4) - (TUTORIAL_BEATMAN_OFFSET / 2) + 20);

	rightBeatmanState1 = Text_CreateSprite();
	Text_Init(&rightBeatmanState1, "..//RhythmGame//$Resources//beatman2.txt");
	Text_Move(&rightBeatmanState1, (GAME_WIDTH / 4) - (TUTORIAL_BEATMAN_OFFSET - 2) + TUTORIAL_MIRROR_OFFSET, (GAME_HEIGHT / 4) - (TUTORIAL_BEATMAN_OFFSET / 2) + 15);

	leftBeatmanState2 = Text_CreateSprite();
	Text_Init(&leftBeatmanState2, "..//RhythmGame//$Resources//beatman1.txt");
	Text_Move(&leftBeatmanState2, (GAME_WIDTH / 4) - (TUTORIAL_BEATMAN_OFFSET - 2) + TUTORIAL_MIRROR_OFFSET, (GAME_HEIGHT / 4) - (TUTORIAL_BEATMAN_OFFSET / 2) + 20);

	rightBeatmanState2 = Text_CreateSprite();
	Text_Init(&rightBeatmanState2, "..//RhythmGame//$Resources//beatman2.txt");
	Text_Move(&rightBeatmanState2, (GAME_WIDTH / 4) - TUTORIAL_BEATMAN_OFFSET, (GAME_HEIGHT / 4) - (TUTORIAL_BEATMAN_OFFSET / 2) + 15);

	Instruction = Text_CreateSprite();
	Text_Init(&Instruction, "..//RhythmGame//$Resources//Instruction_Header.txt");
	Text_Move(&Instruction, (GAME_WIDTH / 2) - TUTORIAL_INSTRUCTION_OFFSET, (GAME_HEIGHT / 2) - (TUTORIAL_BEATMAN_OFFSET / 2) + 15);

	EnterState1 = Text_CreateSprite();
	Text_Init(&EnterState1, "..//RhythmGame//$Resources//Instruction_Enter1.txt");
	Text_Move(&EnterState1, (GAME_WIDTH / 4), (GAME_HEIGHT / 2) + TUTORIAL_ENTER_OFFSET);

	EnterState2 = Text_CreateSprite();
	Text_Init(&EnterState2, "..//RhythmGame//$Resources//Instruction_Enter2.txt");
	Text_Move(&EnterState2, (GAME_WIDTH / 4), (GAME_HEIGHT / 2) + TUTORIAL_ENTER_OFFSET);
}

void GameplaySprite_Init()
{
	BeatheadState1 = Text_CreateSprite();
	Text_Init(&BeatheadState1, "..//RhythmGame//$Resources//beathead1.txt");
	Text_Move(&BeatheadState1, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSETX - 1, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 2);

	BeatheadState2 = Text_CreateSprite();
	Text_Init(&BeatheadState2, "..//RhythmGame//$Resources//beathead2.txt");
	Text_Move(&BeatheadState2, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSETX - 1, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 2);

	MoveDialogue = Text_CreateSprite();
	Text_Init(&MoveDialogue, "..//RhythmGame//$Resources//MoveDialogue.txt");
	Text_Move(&MoveDialogue, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSETX + 25, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 5);

	ArrowKeysState1 = Text_CreateSprite();
	Text_Init(&ArrowKeysState1, "..//RhythmGame//$Resources//ArrowKeys1.txt");
	Text_Move(&ArrowKeysState1, (GAME_WIDTH / 2) + (TUTORIAL_DIALOGUE_OFFSETX / 2), (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY - 5);

	ArrowKeysState2 = Text_CreateSprite();
	Text_Init(&ArrowKeysState2, "..//RhythmGame//$Resources//ArrowKeys2.txt");
	Text_Move(&ArrowKeysState2, (GAME_WIDTH / 2) + (TUTORIAL_DIALOGUE_OFFSETX / 2) + 1, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY - 4);

	DashDialogue = Text_CreateSprite();
	Text_Init(&DashDialogue, "..//RhythmGame//$Resources//DashDialogue.txt");
	Text_Move(&DashDialogue, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSETX + 25, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 5);

	SpaceKeyState1 = Text_CreateSprite();
	Text_Init(&SpaceKeyState1, "..//RhythmGame//$Resources//SpaceKey1.txt");
	Text_Move(&SpaceKeyState1, (GAME_WIDTH / 2) + (TUTORIAL_DIALOGUE_OFFSETX / 2), (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 2);

	SpaceKeyState2 = Text_CreateSprite();
	Text_Init(&SpaceKeyState2, "..//RhythmGame//$Resources//SpaceKey2.txt");
	Text_Move(&SpaceKeyState2, (GAME_WIDTH / 2) + (TUTORIAL_DIALOGUE_OFFSETX / 2) + 1, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 3);

	EnemyDialogue = Text_CreateSprite();
	Text_Init(&EnemyDialogue, "..//RhythmGame//$Resources//EnemyDialogue.txt");
	Text_Move(&EnemyDialogue, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSETX + 35, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 5);

	EnemyWarningState1 = Text_CreateSprite();
	Text_Init(&EnemyWarningState1, "..//RhythmGame//$Resources//EnemySpotted1.txt");
	Text_Move(&EnemyWarningState1, (GAME_WIDTH / 2) - TUTORIAL_ENEMY_POSITIONX, (GAME_HEIGHT / 2) - TUTORIAL_ENEMY_POSITIONY);

	EnemyWarningState2 = Text_CreateSprite();
	Text_Init(&EnemyWarningState2, "..//RhythmGame//$Resources//EnemySpotted2.txt");
	Text_Move(&EnemyWarningState2, (GAME_WIDTH / 2) - TUTORIAL_ENEMY_POSITIONX, (GAME_HEIGHT / 2) - TUTORIAL_ENEMY_POSITIONY);

	NotesDialogue = Text_CreateSprite();
	Text_Init(&NotesDialogue, "..//RhythmGame//$Resources//NotesDialogue.txt");
	Text_Move(&NotesDialogue, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSETX + 25, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 2);

	EndTutorialDialogue = Text_CreateSprite();
	Text_Init(&EndTutorialDialogue, "..//RhythmGame//$Resources//EndTutorialDialogue.txt");
	Text_Move(&EndTutorialDialogue, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSETX + 30, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 2);
}

void _RenderBeatmanAnimation()
{
	animateBeatman = !Audio_GetSpectrum(1);
	if (animateBeatman == false)
	{
		Text_Render(&EnterState2, 0, 0);
		Text_Render(&leftBeatmanState1, 0, 0);
		Text_Render(&rightBeatmanState1, 0, 0);
	}

	if (animateBeatman == true)
	{
		Text_Render(&EnterState1, 0, 0);
		Text_Render(&leftBeatmanState2, 0, 0);
		Text_Render(&rightBeatmanState2, 0, 0);
	}
}

void _RenderBeatHeadAnimation()
{
	animateBeatHead = !Audio_GetSpectrum(0);
	if (animateBeatHead == false)
	{
		Text_Render(&BeatheadState2, 0, 0);
	}

	if (animateBeatHead == true)
	{
		Text_Render(&BeatheadState1, 0, 0);
	}
}

void _RenderEnterAnimation()
{
	animateEnter = !Audio_GetSpectrum(0);
	if (animateEnter == false)
	{
		Text_Render(&EnterState2, 0, 0);
	}

	if (animateEnter == true)
	{
		Text_Render(&EnterState1, 0, 0);
	}
}

void _RenderArrowKeysAnimation()
{
	animateArrowKeys = !Audio_GetSpectrum(0);
	if (animateArrowKeys == false)
	{
		Text_Render(&ArrowKeysState2, 0, 0);
	}

	if (animateArrowKeys == true)
	{
		Text_Render(&ArrowKeysState1, 0, 0);
	}
}

void _RenderSpaceKeyAnimation()
{
	animateSpaceKey = !Audio_GetSpectrum(0);
	if (animateSpaceKey == false)
	{
		Text_Render(&SpaceKeyState2, 0, 0);
	}

	if (animateSpaceKey == true)
	{
		Text_Render(&SpaceKeyState1, 0, 0);
	}
}

void _RenderWarningAnimation()
{
	animateSpaceKey = !Audio_GetSpectrum(0);
	if (animateSpaceKey == false)
	{
		Text_Render(&EnemyWarningState2, 0, 0);
	}

	if (animateSpaceKey == true)
	{
		Text_Render(&EnemyWarningState1, 0, 0);
	}
}

static void _MovePlayer()
{
	// Allows player to move
	if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_UP))
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
	else if (!GetAsyncKeyState(VK_SPACE)) spaceDown = false;
}