#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "raylib.h"

// Forward declarations
typedef struct Enemy Enemy;
typedef struct GameState GameState;

void WrapAroundPlayer(Rectangle *player, int screenWidth);
void WrapAroundEnemy(Enemy *enemy, int screenWidth);
void CheckPlatformCollisions(GameState *state);
void CheckPlayerEnemyCollisions(GameState *state);
void CheckAttackEnemyCollisions(GameState *state);
void CheckFallingDamage(GameState *state, int screenHeight);
void RespawnPlayer(GameState *state);
void CheckPlayerItemCollisions(GameState *state);
void CheckParryProjectileCollisions(GameState *state);
void CheckEnemyFallingDamage(GameState *state, int screenHeight);


#endif
