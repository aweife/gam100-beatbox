/*******************************************************************************
* @filename tutorial.c
* @author(s) Fikrul Islami Bin Abdullah
* @DP emails f.abdullah@digipen.edu
* Brief Description:
* This file contains the code to run the tutorial of the game.
* COPYRIGHT � 2019 BY DIGIPEN CORP, USA. ALL RIGHTS RESERVED.
*******************************************************************************/
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
//Enemy X Coord - 30 && Enemy Y Coord - 5
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
typedef enum TutorialStages
{
	STATE_INSTRUCTION,
	STATE_PLAYERMOVE,
	STATE_PLAYERDASH,
	STATE_ENEMY,
	STATE_NOTES,
	STATE_END,
} TutorialStages;

TutorialStages state;

//Boolean Variables.
static bool spaceDown = false;
static bool RETURN_DOWN = true;
static bool startTutorial = false;
static bool startGame = false;
static bool spawnEnemy = false;
static bool attackEnemy = false;
static bool animateBeatman = false;
static bool animateEnter = false;
static bool animateBeatHead = false;
static bool animateArrowKeys = false;
static bool animateSpaceKey = false;
static bool animateWarning = false;

//Controls Duration of States
static double tutorialDuration = 0.0;
static double animateDuration = 0.0;

/********Internal functions********/

//Initialise the sprites for the tutorial gameplay
void GameplaySprite_Init();

//Initialise the sprites for the instructions
void InstructionSprite_Init();

//Enables movement of player
void _MovePlayer();

//Enables animation of arrow keys between states
void _RenderArrowKeysAnimation();

//Enables animation of BeatHead between states
void _RenderBeatHeadAnimation();

//Enables animation of Beatman between states
void _RenderBeatmanAnimation();

//Enables animation of Beatman between states
void _RenderEnterAnimation();

//Enables animation of Space Key between states
void _RenderSpaceKeyAnimation();

//Enables animation of Warning Sign betweens states
void _RenderWarningAnimation();

//*********************************************************************************
//									INPUT
//*********************************************************************************
void Tutorial_ProcessInput()
{
	if (GetAsyncKeyState(VK_RETURN) && !RETURN_DOWN && state == STATE_INSTRUCTION)
	{
		RETURN_DOWN = true;
		// Everything from here onwards will execute once
		state = STATE_PLAYERMOVE;

		Player_Init(TUT);
		Map_Init(0);
		Attack_Init();
		Audio_Unload(3);
		Audio_Load(MAINMENU);
		Audio_PlayBGM(MAINMENU);
	}
	else if (GetAsyncKeyState(VK_RETURN) && !RETURN_DOWN && startGame == true) {
		// Changes to Main Game
		StateMachine_ChangeMode(0);
		StateMachine_ChangeState(State_Game);
	}
	else if (!GetAsyncKeyState(VK_RETURN)) {
		RETURN_DOWN = false;
	}

	if (state != STATE_INSTRUCTION)
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

	if (state != STATE_INSTRUCTION)
	{
		tutorialDuration += Clock_GetDeltaTime();
		Player_Update(0);
		Map_Update();
	}

	// Moves on after 5 seconds
	if (tutorialDuration >= 0.0 && tutorialDuration <= 5000.0)
	{
		if (!startTutorial)
			startTutorial = true;
	}
	// Moves on after 5 seconds
	else if (tutorialDuration > 5000.0 && tutorialDuration <= 10000.0) {
		state = STATE_PLAYERDASH;
	}
	// Moves on after 20 seconds
	else if (tutorialDuration > 10000.0 && tutorialDuration <= 30000.0) {
		state = STATE_ENEMY;

		if (!spawnEnemy)
		{
			Enemy_Init(TUT);
			spawnEnemy = true;
		}

		Enemy_Update();

		if (!attackEnemy)
		{
			Attack_Init();
			attackEnemy = true;
		}

		Attack_Update();

		if (tutorialDuration > 17000.0 && tutorialDuration <= 30000.0)
		{
			state = STATE_NOTES;
		}

	}
	// Ends Tutorial here
	else if (tutorialDuration > 30000.0) {
		state = STATE_END; 
		spawnEnemy = false;
		attackEnemy = false;

		if (!startGame)
		{
			startGame = true;
		}
	}
}

//*********************************************************************************
//									RENDER
//*********************************************************************************
void Tutorial_Render()
{
	if (state == STATE_INSTRUCTION)
	{
		Text_RenderRainbow(&Instruction);
		_RenderBeatmanAnimation();
		_RenderEnterAnimation();
	}

	if (state == STATE_PLAYERMOVE)
	{
		Map_Render();
		Player_Render(0);
		_RenderBeatHeadAnimation();
		Text_Render(&MoveDialogue, 0, 0);
		_RenderArrowKeysAnimation();
	}

	if (state == STATE_PLAYERDASH)
	{
		Map_Render();
		Player_Render(0);
		_RenderBeatHeadAnimation();
		Text_Render(&DashDialogue, 0, 0);
		_RenderSpaceKeyAnimation();
	}

	if (state == STATE_ENEMY)
	{
		Map_Render();
		Player_Render(0);
		_RenderBeatHeadAnimation();
		Text_Render(&EnemyDialogue, 0, 0);
		_RenderWarningAnimation();
		Enemy_Render();
		Attack_Render();
	}

	if (state == STATE_NOTES)
	{
		Map_Render();
		Player_Render(0);
		_RenderBeatHeadAnimation();
		Text_Render(&NotesDialogue, 0, 0);
		Enemy_Render();
		Attack_Render();
	}

	if (state == STATE_END)
	{
		Map_Render();
		Player_Render(0);
		_RenderBeatHeadAnimation();
		Text_Render(&EndTutorialDialogue, 0, 0);
	}
}

//*********************************************************************************
//								STATE MANAGEMENT
//*********************************************************************************
void Tutorial_EnterState()
{
	spaceDown = false;
	RETURN_DOWN = true;
	startTutorial = false;
	startGame = false;
	spawnEnemy = false;
	attackEnemy = false;
	animateBeatman = false;
	animateEnter = false;
	animateBeatHead = false;
	animateArrowKeys = false;
	animateSpaceKey = false;
	animateWarning = false;
	state = STATE_INSTRUCTION;
	tutorialDuration = 0.0;
	animateDuration = 0.0;
	InstructionSprite_Init();
	GameplaySprite_Init();
}

void Tutorial_ExitState()
{
	//FREE ALL THE SPRITES
	Attack_Cleanup();
	Text_Cleanup(&Instruction); 
	Text_Cleanup(&MoveDialogue);
	Text_Cleanup(&DashDialogue);
	Text_Cleanup(&EnemyDialogue);
	Text_Cleanup(&NotesDialogue);
	Text_Cleanup(&EndTutorialDialogue);
	Text_Cleanup(&leftBeatmanState1);
	Text_Cleanup(&rightBeatmanState1);
	Text_Cleanup(&leftBeatmanState2);
	Text_Cleanup(&rightBeatmanState2);
	Text_Cleanup(&EnterState1);
	Text_Cleanup(&EnterState2);
	Text_Cleanup(&BeatheadState1);
	Text_Cleanup(&BeatheadState2);
	Text_Cleanup(&ArrowKeysState1);
	Text_Cleanup(&ArrowKeysState2);
	Text_Cleanup(&SpaceKeyState1);
	Text_Cleanup(&SpaceKeyState2);
	Text_Cleanup(&EnemyWarningState1);
	Text_Cleanup(&EnemyWarningState2);
}

//*********************************************************************************
//						      INTERNAL FUNCTIONS
//*********************************************************************************

void InstructionSprite_Init()
{
	leftBeatmanState1 = Text_CreateSprite();
	Text_Init(&leftBeatmanState1, "$Resources//Sprites//Tutorial//beatman1.txt");
	Text_Move(&leftBeatmanState1, (GAME_WIDTH / 4) - TUTORIAL_BEATMAN_OFFSET, (GAME_HEIGHT / 4) - (TUTORIAL_BEATMAN_OFFSET / 2) + 20);

	rightBeatmanState1 = Text_CreateSprite();
	Text_Init(&rightBeatmanState1, "$Resources//Sprites//Tutorial//beatman2.txt");
	Text_Move(&rightBeatmanState1, (GAME_WIDTH / 4) - (TUTORIAL_BEATMAN_OFFSET - 2) + TUTORIAL_MIRROR_OFFSET, (GAME_HEIGHT / 4) - (TUTORIAL_BEATMAN_OFFSET / 2) + 15);

	leftBeatmanState2 = Text_CreateSprite();
	Text_Init(&leftBeatmanState2, "$Resources//Sprites//Tutorial//beatman1.txt");
	Text_Move(&leftBeatmanState2, (GAME_WIDTH / 4) - (TUTORIAL_BEATMAN_OFFSET - 2) + TUTORIAL_MIRROR_OFFSET, (GAME_HEIGHT / 4) - (TUTORIAL_BEATMAN_OFFSET / 2) + 20);

	rightBeatmanState2 = Text_CreateSprite();
	Text_Init(&rightBeatmanState2, "$Resources//Sprites//Tutorial//beatman2.txt");
	Text_Move(&rightBeatmanState2, (GAME_WIDTH / 4) - TUTORIAL_BEATMAN_OFFSET, (GAME_HEIGHT / 4) - (TUTORIAL_BEATMAN_OFFSET / 2) + 15);

	Instruction = Text_CreateSprite();
	Text_Init(&Instruction, "$Resources//Sprites//Tutorial//Instruction_Header.txt");
	Text_Move(&Instruction, (GAME_WIDTH / 2) - TUTORIAL_INSTRUCTION_OFFSET, (GAME_HEIGHT / 2) - (TUTORIAL_BEATMAN_OFFSET / 2) + 15);

	EnterState1 = Text_CreateSprite();
	Text_Init(&EnterState1, "$Resources//Sprites//Tutorial//Instruction_Enter1.txt");
	Text_Move(&EnterState1, (GAME_WIDTH / 4), (GAME_HEIGHT / 2) + TUTORIAL_ENTER_OFFSET);

	EnterState2 = Text_CreateSprite();
	Text_Init(&EnterState2, "$Resources//Sprites//Tutorial//Instruction_Enter2.txt");
	Text_Move(&EnterState2, (GAME_WIDTH / 4), (GAME_HEIGHT / 2) + TUTORIAL_ENTER_OFFSET);
}

void GameplaySprite_Init()
{
	BeatheadState1 = Text_CreateSprite();
	Text_Init(&BeatheadState1, "$Resources//Sprites//Tutorial//beathead1.txt");
	Text_Move(&BeatheadState1, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSETX - 1, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 2);

	BeatheadState2 = Text_CreateSprite();
	Text_Init(&BeatheadState2, "$Resources//Sprites//Tutorial//beathead2.txt");
	Text_Move(&BeatheadState2, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSETX - 1, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 2);

	MoveDialogue = Text_CreateSprite();
	Text_Init(&MoveDialogue, "$Resources//Sprites//Tutorial//MoveDialogue.txt");
	Text_Move(&MoveDialogue, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSETX + 25, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 5);

	ArrowKeysState1 = Text_CreateSprite();
	Text_Init(&ArrowKeysState1, "$Resources//Sprites//Tutorial//ArrowKeys1.txt");
	Text_Move(&ArrowKeysState1, (GAME_WIDTH / 2) + (TUTORIAL_DIALOGUE_OFFSETX / 2), (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY - 5);

	ArrowKeysState2 = Text_CreateSprite();
	Text_Init(&ArrowKeysState2, "$Resources//Sprites//Tutorial//ArrowKeys2.txt");
	Text_Move(&ArrowKeysState2, (GAME_WIDTH / 2) + (TUTORIAL_DIALOGUE_OFFSETX / 2) + 1, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY - 4);

	DashDialogue = Text_CreateSprite();
	Text_Init(&DashDialogue, "$Resources//Sprites//Tutorial//DashDialogue.txt");
	Text_Move(&DashDialogue, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSETX + 25, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 5);

	SpaceKeyState1 = Text_CreateSprite();
	Text_Init(&SpaceKeyState1, "$Resources//Sprites//Tutorial//SpaceKey1.txt");
	Text_Move(&SpaceKeyState1, (GAME_WIDTH / 2) + (TUTORIAL_DIALOGUE_OFFSETX / 2), (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 2);

	SpaceKeyState2 = Text_CreateSprite();
	Text_Init(&SpaceKeyState2, "$Resources//Sprites//Tutorial//SpaceKey2.txt");
	Text_Move(&SpaceKeyState2, (GAME_WIDTH / 2) + (TUTORIAL_DIALOGUE_OFFSETX / 2) + 1, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 3);

	EnemyDialogue = Text_CreateSprite();
	Text_Init(&EnemyDialogue, "$Resources//Sprites//Tutorial//EnemyDialogue.txt");
	Text_Move(&EnemyDialogue, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSETX + 35, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 5);

	EnemyWarningState1 = Text_CreateSprite();
	Text_Init(&EnemyWarningState1, "$Resources//Sprites//Tutorial//EnemySpotted1.txt");
	Text_Move(&EnemyWarningState1, (GAME_WIDTH / 2) - TUTORIAL_ENEMY_POSITIONX, (GAME_HEIGHT / 2) - TUTORIAL_ENEMY_POSITIONY);

	EnemyWarningState2 = Text_CreateSprite();
	Text_Init(&EnemyWarningState2, "$Resources//Sprites//Tutorial//EnemySpotted2.txt");
	Text_Move(&EnemyWarningState2, (GAME_WIDTH / 2) - TUTORIAL_ENEMY_POSITIONX, (GAME_HEIGHT / 2) - TUTORIAL_ENEMY_POSITIONY);

	NotesDialogue = Text_CreateSprite();
	Text_Init(&NotesDialogue, "$Resources//Sprites//Tutorial//NotesDialogue.txt");
	Text_Move(&NotesDialogue, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSETX + 25, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 2);

	EndTutorialDialogue = Text_CreateSprite();
	Text_Init(&EndTutorialDialogue, "$Resources//Sprites//Tutorial//EndTutorialDialogue.txt");
	Text_Move(&EndTutorialDialogue, (GAME_WIDTH / 2) - TUTORIAL_DIALOGUE_OFFSETX + 30, (GAME_HEIGHT / 2) + TUTORIAL_DIALOGUE_OFFSETY + 2);
}

void _RenderBeatmanAnimation()
{
	animateBeatman = !Audio_GetSpectrum(1);
	if (animateBeatman == false)
	{
		Text_Render(&leftBeatmanState1, 0, 0);
		Text_Render(&rightBeatmanState1, 0, 0);
	}

	if (animateBeatman == true)
	{
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
		Player_SetVel(TOPLEFT, SpeedUp,0);
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_UP))
		Player_SetVel(TOPRIGHT, SpeedUp,0);
	else if (GetAsyncKeyState(VK_RIGHT) && GetAsyncKeyState(VK_DOWN))
		Player_SetVel(BOTTOMRIGHT, SpeedUp,0);
	else if (GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_DOWN))
		Player_SetVel(BOTTOMLEFT, SpeedUp,0);
	else if (GetAsyncKeyState(VK_UP))
		Player_SetVel(UP, SpeedUp,0);
	else if (GetAsyncKeyState(VK_RIGHT))
		Player_SetVel(RIGHT, SpeedUp,0);
	else if (GetAsyncKeyState(VK_DOWN))
		Player_SetVel(DOWN, SpeedUp,0);
	else if (GetAsyncKeyState(VK_LEFT))
		Player_SetVel(LEFT, SpeedUp,0);
	else
	{
		if (Player_GetDirection(0) != STAY)
		{
			Player_SetVel(Player_GetDirection(0), SlowDown,0);
			if (Player_GetEaseFactor(0) <= 0)
				Player_SetVel(STAY, SlowDown,0);
		}
	}

	if (GetAsyncKeyState(VK_SPACE) && !spaceDown)
	{
		spaceDown = true;
		Player_Dash(0);
	}
	else if (!GetAsyncKeyState(VK_SPACE)) spaceDown = false;
}