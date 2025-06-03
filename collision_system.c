#include "collision_system.h"
#include "game_state.h"  // Include for full definition
#include <math.h>
#include <string.h>

void WrapAroundPlayer(Rectangle *player, int screenWidth) {
    if (player->x + player->width < 0) player->x = screenWidth;
    if (player->x > screenWidth) player->x = -player->width;
}

void WrapAroundEnemy(Enemy *enemy, int screenWidth) {
    if (enemy->hitbox.x + enemy->hitbox.width < 0) enemy->hitbox.x = screenWidth;
    if (enemy->hitbox.x > screenWidth) enemy->hitbox.x = -enemy->hitbox.width;
}

void CheckPlatformCollisions(GameState *state) {
    state->isonGround = false;

    if (state->vel_y >= 0) {
        bool onPlatform = false;

        // Main platform
        if (state->player.y + state->player.height <= state->platform.y + 5 &&
            state->player.y + state->player.height + state->vel_y >= state->platform.y &&
            state->player.x + state->player.width > state->platform.x &&
            state->player.x < state->platform.x + state->platform.width) {
            state->player.y = state->platform.y - state->player.height;
            state->vel_y = 0;
            state->isonGround = true;
            onPlatform = true;
        }
        // Mini platform
        else if (state->player.y + state->player.height <= state->miniPlatform.y + 5 &&
                 state->player.y + state->player.height + state->vel_y >= state->miniPlatform.y &&
                 state->player.x + state->player.width > state->miniPlatform.x &&
                 state->player.x < state->miniPlatform.x + state->miniPlatform.width) {
            state->player.y = state->miniPlatform.y - state->player.height;
            state->vel_y = 0;
            state->isonGround = true;
            onPlatform = true;
        }
        // Left platform (wave 3+)
        else if (state->currentWave >= 3) {
            if (state->player.y + state->player.height <= state->platformLeft.y + 5 &&
                state->player.y + state->player.height + state->vel_y >= state->platformLeft.y &&
                state->player.x + state->player.width > state->platformLeft.x &&
                state->player.x < state->platformLeft.x + state->platformLeft.width) {
                state->player.y = state->platformLeft.y - state->player.height;
                state->vel_y = 0;
                state->isonGround = true;
                onPlatform = true;
            }
            // Right platform (wave 3+)
            else if (state->player.y + state->player.height <= state->platformRight.y + 5 &&
                     state->player.y + state->player.height + state->vel_y >= state->platformRight.y &&
                     state->player.x + state->player.width > state->platformRight.x &&
                     state->player.x < state->platformRight.x + state->platformRight.width) {
                state->player.y = state->platformRight.y - state->player.height;
                state->vel_y = 0;
                state->isonGround = true;
                onPlatform = true;
            }
        }

        if (!onPlatform) state->isonGround = false;
    }
}

void CheckPlayerEnemyCollisions(GameState *state) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (state->enemies[i].alive && CheckCollisionRecs(state->player, state->enemies[i].hitbox)) {
            if (state->playerInvincibleTimer <= 0) {
                state->playerStatus.hp -= state->enemies[i].atk;
                state->playerInvincibleTimer = 0.75f;
                if (state->playerStatus.hp <= 0) {
                    state->finished = true;
                    state->death = 2;
                }
            }
        }
    }
}

void CheckAttackEnemyCollisions(GameState *state) {
    if (!state->isAttacking) return;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!state->enemies[i].alive) continue;

        if (CheckCollisionRecs(state->attack, state->enemies[i].hitbox)) {
            if (state->attackDirection == 'K') state->vel_y = -12.5f;

            if (state->enemies[i].hitCooldownTimer == 0) {
                state->enemies[i].hp -= state->playerStatus.atk;
                state->enemies[i].hitCooldownTimer = 0.3f;

                if (state->enemies[i].hp <= 0) {
                    state->enemies[i].alive = false;
                    state->enemiesDefeated++;
                }
            }
        }
    }
}

void CheckFallingDamage(GameState *state, int screenHeight) {
    if (state->player.y > screenHeight) {
        state->playerStatus.hp--;
        RespawnPlayer(state);
        state->playerInvincibleTimer = 1.0f;

        if (state->playerStatus.hp <= 0) {
            state->finished = true;
            state->death = 2;
        }
    }
}

void RespawnPlayer(GameState *state) {
    state->player.x = state->miniPlatform.x + (state->miniPlatform.width - state->player.width) / 2;
    state->player.y = state->miniPlatform.y - state->player.height;
    state->vel_y = 0;
}
