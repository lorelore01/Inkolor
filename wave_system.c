#include "wave_system.h"
#include "enemy_system.h"
#include "upgrade_system.h"

void UpdateWaveSystem(GameState *state, float delta) {
    if (state->waveStarting) {
        state->waveStartTimer -= delta;
        if (state->waveStartTimer <= 0.0f) {
            state->waveStarting = false;
            SpawnEnemyWave(state);
            state->enemiesAlive = state->currentWave + 1;
        }
    }
}

void HandleEnemyDefeat(GameState *state) {
    if (state->enemiesDefeated >= state->enemiesAlive) {
        state->enemiesDefeated = 0;
        state->enemiesAlive = 0;

        if (state->currentWave == 10) {
            state->finished = true;
            state->death = 1;
            return;
        }

        if ((state->currentWave % 2 == 0) && state->upgradePending) {
            state->onUpgradeScreen = true;
        } else {
            state->currentWave++;

            if (state->currentWave % 2 == 0) {
                state->upgradePending = true;
            }

            state->waveStartTimer = 3.0f;
            state->waveStarting = true;
        }
    }
}
