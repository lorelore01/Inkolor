#include "upgrade_system.h"
#include "game_state.h"

#define MAX_PLAYER_HP 20

void upgradeAtk(GameState *state) {
    state->playerStatus.atk += 1;
}

void upgradeRange(GameState *state) {
    state->attackWidth += 25;
}

void upgradeHp(GameState *state) {
    state->playerStatus.hp += 3;
    if (state->playerStatus.hp > MAX_PLAYER_HP) {
        state->playerStatus.hp = MAX_PLAYER_HP;
    }
}

void ApplyUpgrade(GameState *state) {
    if (state->selectedUpgrade == -1) return;

    state->upgrades[state->selectedUpgrade].apply(state);
    state->selectedUpgrade = -1;
    state->onUpgradeScreen = false;
    state->upgradePending = false;
    state->currentWave++;
    state->waveStartTimer = 2.0f;
    state->waveStarting = true;
}
