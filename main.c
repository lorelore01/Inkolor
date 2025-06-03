// main.c
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


int main() {
    const int screenWidth = 1600;
    const int screenHeight = 920;
    InitWindow(screenWidth, screenHeight, "Tela");
    SetTargetFPS(60);

    // Initialize game state
    GameState state;
    InitializeGameState(&state, screenWidth, screenHeight);
    SetGlobalState(&state); // Agora deve funcionar

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        // Input handling
        if (!state.started) {
            HandleMenuInput(&state, screenWidth);
        }
        else if (state.finished) {
            HandleMenuInput(&state, screenWidth);
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
            if (!state.waveStarting && state.enemy.alive) {
                UpdateEnemy(&state, delta);
            }

            // Collision systems
            CheckPlayerEnemyCollision(&state);
            CheckAttackEnemyCollision(&state);

            // Wave systems
            if (!state.enemy.alive && !state.waveStarting) {
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
