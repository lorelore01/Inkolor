#ifndef ENEMY_SYSTEM_H
#define ENEMY_SYSTEM_H

#include "game_state.h"

// enemy_constants.h


#define GRAVITY_STRENGTH 1.0f
#define RANGED_ENEMY_SAFE_DISTANCE 300.0f
#define RANGED_ENEMY_RETREAT_SPEED 2.0f
#define RANGED_PROJECTILE_SPEED 5.0f
#define RANGED_ENEMY_SHOOT_COOLDOWN 2.0f



void UpdateEnemies(GameState *state, float delta);
void SpawnEnemyWave(GameState *state);

#endif
