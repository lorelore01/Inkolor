#include "input_system.h"
#include "game_state.h"
#include "player_system.h"
#include <math.h>
#include <string.h>

void HandlePlayerInput(GameState *state) {
    // Movement
    if (!state->isDashing) {
        if (IsKeyDown(KEY_A)) {
            state->player.x -= state->playerStatus.speed;
            state->lastMoveDirection = 'A';
        }
        if (IsKeyDown(KEY_D)) {
            state->player.x += state->playerStatus.speed;
            state->lastMoveDirection = 'D';
        }
    }

    // Jump
    if (IsKeyPressed(KEY_SPACE) && state->isonGround) {
        state->vel_y = -25;
    }

    // Attacks
    if (state->attackCooldownTimer <= 0) {
        if (IsKeyPressed(KEY_J)) {
            InitAttack(state, 'J');
        }
        else if (IsKeyPressed(KEY_L)) {
            InitAttack(state, 'L');
        }
        else if (IsKeyPressed(KEY_I)) {
            InitAttack(state, 'I');
        }
        else if (IsKeyPressed(KEY_K)) {
            InitAttack(state, 'K');
        }
    }

    // Dash
    if (IsKeyPressed(KEY_O) && state->dashCooldownTimer <= 0 && !state->isDashing) {
        InitDash(state);
    }

    // Parry
    if (IsKeyPressed(KEY_U) && state->parryCooldownTimer <= 0) {
        InitParry(state);
    }
}

void HandleUpgradeInput(GameState *state) {
    for (int i = 0; i < 3; i++) {
        if (IsKeyPressed(KEY_ONE + i)) {
            state->selectedUpgrade = i;
            break;
        }
    }
}

void HandleMenuInput(GameState *state, int screenWidth) {
    if (IsKeyPressed(KEY_ENTER)) {
        if (!state->started) {
            state->started = true;
            state->player.x = state->miniPlatform.x + (state->miniPlatform.width / 2) - (state->player.width / 2);
            state->player.y = state->miniPlatform.y - state->player.height;
            state->vel_y = 0;
        }
        else if (state->finished && !state->nameInputActive) {
            // Reset game
            state->started = false;
            state->finished = false;
            state->death = 0;
            state->playerStatus.hp = 10;
            state->playerStatus.atk = 1;
            state->playerStatus.speed = 5;
            state->attackWidth = 75;
            state->player.x = state->miniPlatform.x + (state->miniPlatform.width / 2) - (state->player.width / 2);
            state->player.y = state->miniPlatform.y - state->player.height;
            state->vel_y = 0;
            state->currentWave = 1;
            state->waveStarting = true;

            // Reset enemies
            for (int i = 0; i < MAX_ENEMIES; i++) {
                state->enemies[i].alive = false;
            }
        }
    }
}

void HandleNameInput(GameState *state) {
    if (!state->nameInputActive) return;

    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (strlen(state->playerName) < 49)) {
            int len = strlen(state->playerName);
            state->playerName[len] = (char)key;
            state->playerName[len+1] = '\0';
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = strlen(state->playerName);
        if (len > 0) state->playerName[len-1] = '\0';
    }

    if (IsKeyPressed(KEY_ENTER) && strlen(state->playerName) > 0) {
        state->nameInputActive = false;
    }
}
