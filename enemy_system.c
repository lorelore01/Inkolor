#include "enemy_system.h"
#include "collision_system.h"
#include "game_state.h"
#include <math.h>
#include <stdlib.h>

void UpdateEnemies(GameState *state, float delta) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!state->enemies[i].alive) continue;

        // Apply gravity
        state->enemies[i].vel_y += 1;
        state->enemies[i].hitbox.y += state->enemies[i].vel_y;

        // Platform collision
        if (CheckCollisionRecs(state->enemies[i].hitbox, state->platform)) {
            state->enemies[i].hitbox.y = state->platform.y - state->enemies[i].hitbox.height;
            state->enemies[i].vel_y = 0;
        }

        // Movement towards player (only for melee enemies)
        if (state->enemies[i].type == ENEMY_MELEE) {
            if (state->enemies[i].hitbox.x < state->player.x) {
                state->enemies[i].hitbox.x += state->enemies[i].speed;
            } else if (state->enemies[i].hitbox.x > state->player.x) {
                state->enemies[i].hitbox.x -= state->enemies[i].speed;
            }
        }

        // Ranged enemy behavior
        if (state->enemies[i].type == ENEMY_RANGED) {
            // Keep distance from player
            float dist = fabsf(state->player.x - state->enemies[i].hitbox.x);
            if (dist < 300) {
                if (state->player.x < state->enemies[i].hitbox.x) {
                    state->enemies[i].hitbox.x += 2;
                } else {
                    state->enemies[i].hitbox.x -= 2;
                }
            }

            // Shoot projectiles
            state->enemies[i].shootCooldown -= delta;
            if (state->enemies[i].shootCooldown <= 0) {
                // Create projectile
                for (int j = 0; j < MAX_PROJECTILES; j++) {
                    if (!state->projectiles[j].active) {
                        state->projectiles[j].active = true;
                        state->projectiles[j].x = state->enemies[i].hitbox.x + state->enemies[i].hitbox.width/2;
                        state->projectiles[j].y = state->enemies[i].hitbox.y + state->enemies[i].hitbox.height/2;
                        state->projectiles[j].deflected = false;

                        // Calculate direction to player
                        float dx = state->player.x - state->projectiles[j].x;
                        float dy = state->player.y - state->projectiles[j].y;
                        float length = sqrtf(dx*dx + dy*dy);

                        state->projectiles[j].dx = dx/length * 5;
                        state->projectiles[j].dy = dy/length * 5;
                        state->enemies[i].shootCooldown = 2.0f;
                        break;
                    }
                }
            }
        }

        WrapAroundEnemy(&state->enemies[i], GetScreenWidth());
        state->enemies[i].hitCooldownTimer -= delta;
        if (state->enemies[i].hitCooldownTimer < 0) state->enemies[i].hitCooldownTimer = 0;
    }
}

void SpawnEnemyWave(GameState *state) {
    int enemiesToSpawn = state->currentWave + 1;
    if (enemiesToSpawn > MAX_ENEMIES) enemiesToSpawn = MAX_ENEMIES;

    for (int i = 0; i < enemiesToSpawn; i++) {
        state->enemies[i].alive = true;

        // Determine enemy type based on wave
        if (state->currentWave < 3) {
            state->enemies[i].type = ENEMY_MELEE;
        } else if (state->currentWave < 6) {
            state->enemies[i].type = (rand() % 100 < 70) ? ENEMY_MELEE : ENEMY_RANGED;
        } else {
            int randType = rand() % 100;
            if (randType < 50) state->enemies[i].type = ENEMY_MELEE;
            else if (randType < 85) state->enemies[i].type = ENEMY_RANGED;
            else state->enemies[i].type = ENEMY_TANK;
        }

        // Set stats based on type and wave
        switch (state->enemies[i].type) {
            case ENEMY_MELEE:
                state->enemies[i].hp = 3 + (state->currentWave / 2);
                state->enemies[i].atk = 1;
                state->enemies[i].speed = 4;
                break;
            case ENEMY_RANGED:
                state->enemies[i].hp = 2;
                state->enemies[i].atk = 2;
                state->enemies[i].speed = 3;
                state->enemies[i].shootCooldown = 2.0f;
                break;
            case ENEMY_TANK:
                state->enemies[i].hp = 10 + state->currentWave * 2;
                state->enemies[i].atk = 3;
                state->enemies[i].speed = 2;
                break;
        }

        // Position enemies
        state->enemies[i].hitbox.x = 300 + i * 200;
        state->enemies[i].hitbox.y = 770;
        state->enemies[i].hitbox.width = 50;
        state->enemies[i].hitbox.height = 50;
        state->enemies[i].vel_y = 0;
        state->enemies[i].hitCooldownTimer = 0;
    }

    state->enemiesAlive = enemiesToSpawn;
    state->enemiesDefeated = 0;
}
