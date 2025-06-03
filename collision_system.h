// collision_system.h
#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "game_state.h"

void WrapAroundPlayer(Rectangle *player, int screenWidth);
void WrapAroundEnemy(Enemy *enemy, int screenWidth);
void CheckPlatformCollisions(GameState *state);
void CheckPlayerEnemyCollision(GameState *state);
void CheckAttackEnemyCollision(GameState *state);

#endif
