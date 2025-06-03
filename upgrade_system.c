#include "upgrade_system.h"
#include "game_state.h"
#include <stddef.h>
#include <assert.h>

#define MAX_PLAYER_HP 10

static GameState* globalState = NULL;

void SetGlobalState(GameState* state) {
    globalState = state;
}

void upgradeAtk(void) {
    assert(globalState != NULL);
    globalState->playerStatus.atk += 1;
}

void upgradeRange(void) {
    assert(globalState != NULL);
    globalState->attackWidth += 25;
}

void upgradeHp(void) {
    assert(globalState != NULL);
    globalState->playerStatus.hp += 3;
    if (globalState->playerStatus.hp > MAX_PLAYER_HP) {
        globalState->playerStatus.hp = MAX_PLAYER_HP;
    }
}

void ApplyUpgrade(GameState *state) {
    if (!state || state->selectedUpgrade == -1) return;

    state->upgrades[state->selectedUpgrade].apply();
    state->selectedUpgrade = -1;
    state->onUpgradeScreen = false;
    state->upgradePending = false;
    state->currentWave++;
    state->waveStartTimer = 2.0f;
    state->waveStarting = true;
}
