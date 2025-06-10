#include "raylib.h"
#include "game_state.h"
#include "input_system.h"
#include "collision_system.h"
#include "wave_system.h"
#include "player_system.h"
#include "enemy_system.h"
#include "upgrade_system.h"
#include "ui_system.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main() {
    const int screenWidth = 1600;
    const int screenHeight = 920;
    InitWindow(screenWidth, screenHeight, "Inkolor: Platform Fighter");
    SetTargetFPS(60);
    srand(time(NULL)); // Seed for random numbers

    GameState state;
    InitializeGameState(&state, screenWidth, screenHeight);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        if (!state.started) {
            HandleMenuInput(&state, screenWidth);
        }
        else if (state.finished) {
            HandleMenuInput(&state, screenWidth);
            HandleNameInput(&state);
        }
        else {
            if (!state.onUpgradeScreen) {
                HandlePlayerInput(&state);
            } else {
                HandleUpgradeInput(&state);
            }
        }

        // Game logic
        if (state.started && !state.finished) {
            // Player systems
            ApplyPlayerGravity(&state);
            CheckPlatformCollisions(&state);
            UpdatePlayer(&state, delta);

            // Enemy systems
            if (!state.waveStarting && state.enemiesAlive > 0) {
                UpdateEnemies(&state, delta);
            }

            // Collision systems
            CheckPlayerEnemyCollisions(&state);
            CheckAttackEnemyCollisions(&state);
            CheckFallingDamage(&state, screenHeight);
            CheckParryProjectileCollisions(&state);
            CheckPlayerItemCollisions(&state);

            // Projectile system
            UpdateProjectiles(&state);

            // Item system
            UpdateItems(&state, delta);

            // Wave systems
            if (state.enemiesDefeated >= state.enemiesAlive && !state.waveStarting) {
                HandleEnemyDefeat(&state);
            }

            // Upgrade system
            if (state.onUpgradeScreen) {
                ApplyUpgrade(&state);
            }

            // Wave timer
            UpdateWaveSystem(&state, delta);
        }

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        if (!state.started) {
            DrawMenuScreen(&state, screenWidth, screenHeight);
        }
        else if (state.finished) {
            DrawGameOverScreen(&state, screenWidth, screenHeight);
        }
        else {
            if (state.onUpgradeScreen) {
                DrawUpgradeScreen(&state, screenWidth, screenHeight);
            }
            else {
                DrawGameWorld(&state);
                DrawGameUI(&state);
            }
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
