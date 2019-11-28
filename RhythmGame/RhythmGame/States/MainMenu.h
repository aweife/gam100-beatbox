#pragma once

extern void MainMenu_EnterState();
extern void MainMenu_ExitState();
extern void MainMenu_ProcessInput();
extern void MainMenu_Update();
extern void MainMenu_Render();

extern void _renderChoice(int choice);
extern void _confirmChoice(int choice);
extern void _moveToBeat();
extern void _updateTimer();
extern void _renderBeat();