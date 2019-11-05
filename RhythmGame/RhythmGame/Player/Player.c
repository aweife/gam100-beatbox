#include "Player.h"

Player *playerPtr, player;

void playerInit()
{
	pPosX = 40.0;
	pPosY = 40.0;
	direction = 0;
	dt = 0.0;
	velocity = 0.04f;

	factor = 0.0;
	EaseTimer = 0.0;
	EaseBool = 0;
	EaseCheck = 1;

	playerPtr = &player;
}

void playerSetDirection(int dir)
{
	direction = dir;
}

int playerGetDirection()
{
	return direction;
}


void playerMove(int direction)
{
	if (EaseTimer >= 25.0)
	{
		if (factor <= 1.0 && EaseCheck == SpeedUp)
			factor += 0.1;
		else if (factor >= 0.0 && EaseCheck == SlowDown)
			factor -= 0.1;

		EaseTimer = 0.0;
	}

	switch (direction)
	{
	case 1:
		pPosX += -1 * dt * velocity * factor;
		pPosY += -1 * dt * velocity * factor;
		diagonalCheck();
		break;
	case 2:
		pPosX += 1 * dt * velocity * factor;
		pPosY += -1 * dt * velocity * factor;
		break;
	case 3:
		pPosX += 1 * dt * velocity * factor;
		pPosY += 1 * dt * velocity * factor;
		break;
	case 4:
		pPosX += -1 * dt * velocity * factor;
		pPosY += 1 * dt * velocity * factor;
		break;
	case 5: pPosY += -1 * dt * velocity * factor; break;
	case 6: pPosX += 1 * dt * velocity * factor; break;
	case 7: pPosY += 1 * dt * velocity * factor; break;
	case 8: pPosX += -1 * dt * velocity * factor; break;
	default: break;
	}

	playerPtr->originX = pPosX;
	playerPtr->originY = pPosY;
}

void diagonalCheck()
{
	if ((int)pPosX != round(pPosX))
	{
		if ((int)pPosY == round(pPosY))
		{
			pPosX = round(pPosX);
		}
	}
	else
	{
		if ((int)pPosY != round(pPosY))
		{
			pPosY = round(pPosY);
		}
	}
}

void playerShape()
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

Player* playerGetPosition()
{
	return playerPtr;
}