#ifndef ENEMY_SYSTEM_H
#define ENEMY_SYSTEM_H

// Forward declaration
typedef struct GameState GameState;

void UpdateEnemies(GameState *state, float delta);
void SpawnEnemyWave(GameState *state);

#endif
