// wave_system.c
#include "wave_system.h"
#include "upgrade_system.h"

void UpdateWaveSystem(GameState *state, float delta) {
    if (state->waveStarting) {
        state->waveStartTimer -= delta;
        if (state->waveStartTimer <= 0.0f) {
            state->waveStarting = false;
            state->enemy.alive = true;
            state->enemy.hp = 3;
            state->enemy.hitbox.x = 800;
            state->enemy.hitbox.y = 770;
            state->enemy.vel_y = 0;
        }
    }
}

void HandleEnemyDefeat(GameState *state) {
    state->enemiesDefeated++;

    if (state->enemiesDefeated >= state->enemiesPerWave) {
        state->enemiesDefeated = 0;

        if ((state->currentWave == 2 || state->currentWave == 4) && state->upgradePending) {
            state->onUpgradeScreen = true;
        }
        else {
            state->currentWave++;
            if (state->currentWave > 5) {
                state->finished = true;
                state->death = 1;
            } else {
                if (state->currentWave == 2 || state->currentWave == 4) {
                    state->upgradePending = true;
                }
                state->waveStartTimer = 2.0f;
                state->waveStarting = true;
            }
        }
    } else {
        state->enemy.alive = true;
        state->enemy.hp = 3;
        state->enemy.hitbox.x = 800;
        state->enemy.hitbox.y = 770;
        state->enemy.vel_y = 0;
    }
}
