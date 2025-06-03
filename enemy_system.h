#ifndef ENEMY_SYSTEM_H
#define ENEMY_SYSTEM_H

#include "game_state.h"

void UpdateEnemies(GameState *state, float delta);
void SpawnEnemyWave(GameState *state);

#endif
