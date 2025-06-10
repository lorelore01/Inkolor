#include "enemy_system.h"
#include "collision_system.h"
#include "game_state.h"
#include <math.h>
#include <stdlib.h>

void UpdateEnemies(GameState *state, float delta) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!state->enemies[i].alive) continue;

        state->enemies[i].vel_y += 1;
        state->enemies[i].hitbox.y += state->enemies[i].vel_y;

        if (CheckCollisionRecs(state->enemies[i].hitbox, state->platform)) {
            state->enemies[i].hitbox.y = state->platform.y - state->enemies[i].hitbox.height;
            state->enemies[i].vel_y = 0;
        }

        if (state->enemies[i].type == ENEMY_MELEE) {
            if (state->enemies[i].hitbox.x < state->player.x) {
                state->enemies[i].hitbox.x += state->enemies[i].speed;
            } else if (state->enemies[i].hitbox.x > state->player.x) {
                state->enemies[i].hitbox.x -= state->enemies[i].speed;
            }
        }


        if (state->enemies[i].type == ENEMY_RANGED) {
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
    int screenWidth = GetScreenWidth();
    int enemiesToSpawn = state->currentWave + 1;
    if (enemiesToSpawn > MAX_ENEMIES) enemiesToSpawn = MAX_ENEMIES;

    for (int i = 0; i < enemiesToSpawn; i++) {
        state->enemies[i].alive = true;

        if (state->currentWave < 3) {
            state->enemies[i].type = ENEMY_MELEE;
        } else {
            state->enemies[i].type = (rand() % 100 < 70) ? ENEMY_MELEE : ENEMY_RANGED;
        }

        switch (state->enemies[i].type) {
            case ENEMY_MELEE:
                state->enemies[i].hp = 2 + (state->currentWave / 2);
                state->enemies[i].atk = 1;
                state->enemies[i].speed = 4;
                break;
            case ENEMY_RANGED:
                state->enemies[i].hp = 1 + (state->currentWave / 3);
                state->enemies[i].atk = 2;
                state->enemies[i].speed = 0;
                state->enemies[i].shootCooldown = 2.0f;
                break;
        }

        float xSpacing = (screenWidth - 600) / (enemiesToSpawn - 1);
        state->enemies[i].hitbox.x = 300 + (i * xSpacing);
        state->enemies[i].hitbox.y = 770;

        if (state->enemies[i].hitbox.x < 50) state->enemies[i].hitbox.x = 50;
        if (state->enemies[i].hitbox.x > screenWidth - 100)
            state->enemies[i].hitbox.x = screenWidth - 100;

        state->enemies[i].hitbox.width = 50;
        state->enemies[i].hitbox.height = 50;
        state->enemies[i].vel_y = 0;
        state->enemies[i].hitCooldownTimer = 0;
    }

    state->enemiesAlive = enemiesToSpawn;
    state->enemiesDefeated = 0;
}


void UpdateProjectiles(GameState *state) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!state->projectiles[i].active) continue;

        state->projectiles[i].x += state->projectiles[i].dx;
        state->projectiles[i].y += state->projectiles[i].dy;

        if (CheckCollisionPointRec((Vector2){state->projectiles[i].x, state->projectiles[i].y},
                                   state->player) &&
            state->playerInvincibleTimer <= 0) {
            state->playerStatus.hp--;
            state->playerInvincibleTimer = 1.0f;
            state->projectiles[i].active = false;

            if (state->playerStatus.hp <= 0) {
                state->finished = true;
                state->death = 2;
            }
            continue;
        }


        if (state->projectiles[i].deflected) {
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (state->enemies[j].alive &&
                    CheckCollisionPointRec((Vector2){state->projectiles[i].x, state->projectiles[i].y},
                                           state->enemies[j].hitbox)) {
                    state->enemies[j].hp -= 1;
                    state->projectiles[i].active = false;
                    break;
                }
            }
        }

        if (state->projectiles[i].x < 0 || state->projectiles[i].x > GetScreenWidth() ||
            state->projectiles[i].y < 0 || state->projectiles[i].y > GetScreenHeight()) {
            state->projectiles[i].active = false;
        }
    }
}

