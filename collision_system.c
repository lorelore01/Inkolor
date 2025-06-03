// collision_system.c
#include "collision_system.h"
#include "game_state.h"
#include <math.h>

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

        if (state->player.y + state->player.height <= state->platform.y + 5 &&
            state->player.y + state->player.height + state->vel_y >= state->platform.y &&
            state->player.x + state->player.width > state->platform.x &&
            state->player.x < state->platform.x + state->platform.width) {
            state->player.y = state->platform.y - state->player.height;
            state->vel_y = 0;
            state->isonGround = true;
            onPlatform = true;
        }
        else if (state->player.y + state->player.height <= state->miniPlatform.y + 5 &&
                 state->player.y + state->player.height + state->vel_y >= state->miniPlatform.y &&
                 state->player.x + state->player.width > state->miniPlatform.x &&
                 state->player.x < state->miniPlatform.x + state->miniPlatform.width) {
            state->player.y = state->miniPlatform.y - state->player.height;
            state->vel_y = 0;
            state->isonGround = true;
            onPlatform = true;
        }
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

void CheckPlayerEnemyCollision(GameState *state) {
    if (state->enemy.alive && CheckCollisionRecs(state->player, state->enemy.hitbox)) {
        if (state->playerInvincibleTimer <= 0) {
            state->playerStatus.hp -= state->enemy.atk;
            state->playerInvincibleTimer = 0.75f;
            if (state->playerStatus.hp <= 0) {
                state->finished = true;
                state->death = 2;
            }
        }
    }
}

void CheckAttackEnemyCollision(GameState *state) {
    if (state->isAttacking && state->enemy.alive) {
        if (CheckCollisionRecs(state->attack, state->enemy.hitbox)) {
            if (state->attackDirection == 'K') state->vel_y = -12.5f; // jump_power / 2

            if (state->enemy.hitCooldownTimer == 0) {
                state->enemy.hp -= state->playerStatus.atk;
                state->enemy.hitCooldownTimer = 0.3f;

                if (state->enemy.hp <= 0) {
                    state->enemy.alive = false;
                }
            }
        }
    }
}
