#pragma once

//Initialise/re-initialise all variables when entering tutorial state
extern void Tutorial_EnterState();

//Clean up existing values when exiting tutorial
extern void Tutorial_ExitState();

//Provide transition and input to tutorial state
extern void Tutorial_ProcessInput();

//Allows update of variables and sprites in tutorial state
extern void Tutorial_Update();

//Renders all sprites and animation in tutorial state
extern void Tutorial_Render();