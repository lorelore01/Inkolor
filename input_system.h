// input_system.h
#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "game_state.h"

void HandlePlayerInput(GameState *state);
void HandleUpgradeInput(GameState *state);
void HandleMenuInput(GameState *state, int screenWidth);

#endif
