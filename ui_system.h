#ifndef UI_SYSTEM_H
#define UI_SYSTEM_H

#include "game_state.h"

void DrawCooldownBar(int x, int y, int width, int height, float cooldownTimer, float cooldownMax, Color color);
void DrawGameUI(const GameState *state);
void DrawMenuScreen(const GameState *state, int screenWidth, int screenHeight);
void DrawGameOverScreen(const GameState *state, int screenWidth, int screenHeight);
void DrawUpgradeScreen(const GameState *state, int screenWidth, int screenHeight);
void DrawGameWorld(const GameState *state);

#endif
