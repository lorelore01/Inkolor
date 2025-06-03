#include "player_system.h"
#include "collision_system.h"  // Include for WrapAroundPlayer
#include "game_state.h"        // Include for full definition
#include <string.h>            // Include for strlen in some functions

void ApplyPlayerGravity(GameState *state) {
    state->vel_y += 1;
    state->player.y += state->vel_y;
    WrapAroundPlayer(&state->player, GetScreenWidth());
}

void UpdatePlayer(GameState *state, float delta) {
    // Cooldowns
    state->attackCooldownTimer -= delta;
    if (state->attackCooldownTimer < 0) state->attackCooldownTimer = 0;

    state->attackDurationTimer -= delta;
    if (state->attackDurationTimer < 0) {
        state->attackDurationTimer = 0;
        state->isAttacking = false;
    }

    state->parryCooldownTimer -= delta;
    if (state->parryCooldownTimer < 0) state->parryCooldownTimer = 0;

    state->parryTimer -= delta;
    if (state->parryTimer < 0) {
        state->parryTimer = 0;
        state->isParrying = false;
    }

    state->dashCooldownTimer -= delta;
    if (state->dashCooldownTimer < 0) state->dashCooldownTimer = 0;

    state->playerInvincibleTimer -= delta;
    if (state->playerInvincibleTimer < 0) state->playerInvincibleTimer = 0;

    // Dash update
    if (state->isDashing) {
        if (state->dashDirection == 'A') state->player.x -= state->dashSpeed;
        else if (state->dashDirection == 'D') state->player.x += state->dashSpeed;

        WrapAroundPlayer(&state->player, GetScreenWidth());
        state->dashDurationTimer -= delta;

        if (state->dashDurationTimer <= 0) {
            state->isDashing = false;
        }
    }
}

void InitAttack(GameState *state, char direction) {
    switch (direction) {
        case 'J': // Left
            state->attack = (Rectangle){
                state->player.x - state->attackWidth,
                state->player.y + (state->player.height / 2 - state->attackHeight / 2),
                state->attackWidth,
                state->attackHeight
            };
            break;

        case 'L': // Right
            state->attack = (Rectangle){
                state->player.x + state->player.width,
                state->player.y + (state->player.height / 2 - state->attackHeight / 2),
                state->attackWidth,
                state->attackHeight
            };
            break;

        case 'I': // Up
            state->attack = (Rectangle){
                state->player.x + (state->player.width / 2 - state->attackHeight / 2),
                state->player.y - state->attackWidth,
                state->attackHeight,
                state->attackWidth
            };
            break;

        case 'K': // Down
            state->attack = (Rectangle){
                state->player.x + (state->player.width / 2 - state->attackHeight / 2),
                state->player.y + state->player.height,
                state->attackHeight,
                state->attackWidth
            };
            break;
    }

    state->isAttacking = true;
    state->attackCooldownTimer = state->attackCooldown;
    state->attackDurationTimer = state->attackDuration;
    state->attackDirection = direction;
}

void InitDash(GameState *state) {
    state->isDashing = true;
    state->dashDurationTimer = state->dashDuration;
    state->dashCooldownTimer = state->dashCooldown;
    state->playerInvincibleTimer = state->dashDuration;

    if (IsKeyDown(KEY_A)) state->dashDirection = 'A';
    else if (IsKeyDown(KEY_D)) state->dashDirection = 'D';
    else state->dashDirection = state->lastMoveDirection;
}

void InitParry(GameState *state) {
    state->isParrying = true;
    state->parryTimer = state->parryDuration;
    state->parryCooldownTimer = state->parryCooldown;
    state->playerInvincibleTimer = state->parryDuration;
}
