// enemy_system.c
#include "enemy_system.h"
#include "collision_system.h"
#include <math.h>

void UpdateEnemy(GameState *state, float delta) {
    if (state->enemy.alive) {
        // Gravity
        state->enemy.vel_y += 1; // gravity
        state->enemy.hitbox.y += state->enemy.vel_y;

        // Platform collision
        if (CheckCollisionRecs(state->enemy.hitbox, state->platform)) {
            state->enemy.hitbox.y = state->platform.y - state->enemy.hitbox.height;
            state->enemy.vel_y = 0;
        }

        // Movement towards player
        if (state->enemy.hitbox.x < state->player.x) {
            state->enemy.hitbox.x += state->enemy.speed;
        }
        else if (state->enemy.hitbox.x > state->player.x) {
            state->enemy.hitbox.x -= state->enemy.speed;
        }

        // Jump logic
        if (state->player.y + state->player.height < state->enemy.hitbox.y &&
            fabsf(state->player.x - state->enemy.hitbox.x) < 100 &&
            state->enemy.vel_y == 0) {
            state->enemy.vel_y = -16.67f; // jump_power / 1.5
        }

        WrapAroundEnemy(&state->enemy, GetScreenWidth());
        state->enemy.hitCooldownTimer -= delta;
        if (state->enemy.hitCooldownTimer < 0) state->enemy.hitCooldownTimer = 0;
    }
}
