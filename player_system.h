// player_system.h
#ifndef PLAYER_SYSTEM_H
#define PLAYER_SYSTEM_H

#include "game_state.h"

void UpdatePlayer(GameState *state, float delta);
void InitAttack(GameState *state, char direction);
void InitDash(GameState *state);
void InitParry(GameState *state);
void ApplyPlayerGravity(GameState *state);

#endif
