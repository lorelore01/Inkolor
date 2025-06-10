#ifndef ENEMY_SYSTEM_H
#define ENEMY_SYSTEM_H

#include "game_state.h"

#define GRAVITY_STRENGTH 1.0f
#define RANGED_PROJECTILE_SPEED 5.0f
#define RANGED_ENEMY_SHOOT_COOLDOWN 2.0f

void UpdateEnemies(GameState *state, float delta);
void UpdateProjectiles(GameState *state);
void SpawnEnemyWave(GameState *state);

#endif
