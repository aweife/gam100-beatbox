#include "Player.h"

Player *playerPtr, player;

void p_playerInit()
{
	player.direction = 0;
	player.eulerX = 40;
	player.eulerY = 40;
	factor = 0.0;
	EaseBool = 0;
	EaseCheck = SlowDown;

	playerPtr = &player;
}

void _playerSetVel(int dir, int EaseC)
{
	player.direction = dir;
	EaseCheck = EaseC;
}

double _playerGetEaseFactor()
{
	return factor;
}

int _playerGetDirection()
{
	return player.direction;
}

void p_Render()
{
	_playerShape();
	for (int i = 0; i < 9; i++)
	{
		Console_SetRenderBuffer_CharColor(player.playerX[i], player.playerY[i], ' ', BACKGROUND_INTENSITY);
	}
}

void p_playerMove()
{
	dt = Clock_GetDeltaTime();
	EaseTimer += Clock_GetDeltaTime();

	if (EaseTimer >= 25.0)
	{
		
		if (EaseCheck == SpeedUp)
			factor += 0.1;
		else if (EaseCheck == SlowDown)
			factor -= 0.1;
		
		if (factor < 0)
			factor = 0.0;
		else if (factor > 1.0)
			factor = 1.0;
		EaseTimer -= EaseTimer;
	}

	switch (player.direction)
	{
	case 1:
		player.eulerX += -1 * dt * velocity * factor;
		player.eulerY += -1 * dt * velocity * factor;
		break;
	case 2:
		player.eulerX += 1 * dt * velocity * factor;
		player.eulerY += -1 * dt * velocity * factor;
		break;
	case 3:
		player.eulerX += 1 * dt * velocity * factor;
		player.eulerY += 1 * dt * velocity * factor;
		break;
	case 4:
		player.eulerX += -1 * dt * velocity * factor;
		player.eulerY += 1 * dt * velocity * factor;
		break;
	case 5: player.eulerY += -1 * dt * velocity * factor; break;
	case 6: player.eulerX += 1 * dt * velocity * factor; break;
	case 7: player.eulerY += 1 * dt * velocity * factor; break;
	case 8: player.eulerX += -1 * dt * velocity * factor; break;
	default: break;
	}

	player.originX = player.eulerX;
	player.originY = player.eulerY;
}

void _playerShape()
{
	int localx = 0;
	int localy = 0;

	localy = playerPtr->originY--;
	localx = playerPtr->originX--;

	for (int i = 0; i < boxSize; i++)
	{
		for (int j = 0; j < boxSize; j++)
		{
			playerPtr->playerX[i * 3 + j] = localx + j;
			playerPtr->playerY[i * 3 + j] = localy + i;
		}
	}
}


Player* playerGetInfo()
{
	return playerPtr;
}