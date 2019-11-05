#ifndef GAME2_H
#define GAME2_H

void Game_EnterState();
void Game_ExitState();
void Game_ProcessInput();
void Game_Update();
void Game_Render();

void playerMove();
void diagonalCheck();
void playerShape();

void gameRender();

#endif