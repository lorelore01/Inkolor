#include "game_state.h"
#include <string.h>
#include <stdio.h>

void upgradeAtk(GameState*);
void upgradeRange(GameState*);
void upgradeHp(GameState*);

void InitializeGameState(GameState *state, int screenWidth, int screenHeight) {
    // Player
    state->player = (Rectangle){100, 500, 50, 50};
    state->playerStatus.hp = 10;
    state->playerStatus.atk = 1;
    state->playerStatus.speed = 5;
    state->playerStatus.speedTimer = 0.0f;
    state->playerStatus.originalSpeed = 5;
    state->vel_y = 0;
    state->isonGround = false;
    state->playerInvincibleTimer = 0;
    state->lastMoveDirection = 'D';
    state->attackWidth = 75;
    state->attackHeight = 30;

    // Attack
    state->attack = (Rectangle){0, 0, 0, 0};
    state->attackCooldown = 0.5f;
    state->attackCooldownTimer = 0;
    state->attackDuration = 0.15f;
    state->attackDurationTimer = 0;
    state->isAttacking = false;
    state->attackDirection = ' ';

    // Parry
    state->parryDuration = 0.75f;
    state->parryTimer = 0;
    state->isParrying = false;
    state->parryCooldown = 3.0f;
    state->parryCooldownTimer = 0;

    // Dash
    state->dashCooldown = 1.5f;
    state->dashCooldownTimer = 0;
    state->dashSpeed = 20;
    state->dashDuration = 0.2f;
    state->dashDurationTimer = 0;
    state->isDashing = false;
    state->dashDirection = ' ';

    // Enemies
    for (int i = 0; i < MAX_ENEMIES; i++) {
        state->enemies[i].alive = false;
    }
    state->enemiesAlive = 0;

    // Projectiles
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        state->projectiles[i].active = false;
    }

    // Items
    for (int i = 0; i < MAX_ITEMS; i++) {
        state->items[i].active = false;
    }


    // Platforms
    state->platform = (Rectangle){-100, 820, 1700, 50};
    state->miniPlatform = (Rectangle){screenWidth / 2 - 75, 525, 150, 20};
    state->platformLeft = (Rectangle){100, 525, 300, 20};
    state->platformRight = (Rectangle){1200, 525, 300, 20};

    // Game flow
    state->started = false;
    state->finished = false;
    state->death = 0;

    // Wave system
    state->currentWave = 1;
    state->enemiesDefeated = 0;
    state->waveStartTimer = 0.0f;
    state->waveStarting = true;

    // Upgrade
    state->onUpgradeScreen = false;
    state->selectedUpgrade = -1;
    state->upgradePending = false;

    // Initialize upgrades with correct signatures
    state->upgrades[0] = (Upgrade){"+1 Dano", upgradeAtk};
    state->upgrades[1] = (Upgrade){"+25 de alcance", upgradeRange};
    state->upgrades[2] = (Upgrade){"+3 de vida", upgradeHp};

    // Player name
    strcpy(state->playerName, "");
    state->nameInputActive = false;
}


void UpdateItems(GameState *state, float delta) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!state->items[i].active) continue;

        state->items[i].lifeTimer -= delta;
        if (state->items[i].lifeTimer <= 0) {
            state->items[i].active = false;
        }
    }
}
