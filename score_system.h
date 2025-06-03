// score_system.h
#ifndef SCORE_SYSTEM_H
#define SCORE_SYSTEM_H

#include "game_state.h"

void SaveHighScore(const char* name, int wave);
void LoadHighScores(GameState *state);
void UpdateProjectiles(GameState *state);

#endif
