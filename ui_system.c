// ui_system.c
#include "ui_system.h"
#include "game_state.h"
#include "raylib.h"

void DrawCooldownBar(int x, int y, int width, int height, float cooldownTimer, float cooldownMax, Color color) {
    float percent = 1.0f;
    if (cooldownMax > 0) percent = 1.0f - (cooldownTimer / cooldownMax);
    if (percent < 0) percent = 0;
    if (percent > 1) percent = 1;
    DrawRectangle(x, y, width, height, DARKGRAY);
    DrawRectangle(x, y, (int)(width * percent), height, color);
    DrawRectangleLines(x, y, width, height, BLACK);
}

void DrawGameUI(const GameState *state) {
    // HP bar
    int barWidth = 200;
    int barHeight = 20;
    int barX = 50;
    int barY = 50;
    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    int hpWidth = (state->playerStatus.hp * barWidth) / 10;
    DrawRectangle(barX, barY, hpWidth, barHeight, RED);
    DrawText(TextFormat("HP: %d / 10", state->playerStatus.hp), barX, barY - 25, 20, WHITE);

    // Enemy HP bar
    if (state->enemy.alive) {
        int enemyHpMax = 3;
        int enemyBarWidth = (int)(state->enemy.hitbox.width * ((float)state->enemy.hp / enemyHpMax));
        DrawRectangle((int)state->enemy.hitbox.x, (int)(state->enemy.hitbox.y + state->enemy.hitbox.height + 5),
                     enemyBarWidth, 5, RED);
        DrawRectangleLines((int)state->enemy.hitbox.x, (int)(state->enemy.hitbox.y + state->enemy.hitbox.height + 5),
                          (int)state->enemy.hitbox.width, 5, BLACK);
    }

    // Cooldown bars
    int hudX = 50;
    int hudY = 80;
    int cooldownBarWidth = 100;
    int cooldownBarHeight = 15;
    int hudGap = 75;

    DrawText("ATK", hudX, hudY, 30, GREEN);
    DrawCooldownBar(hudX, hudY + 35, cooldownBarWidth, cooldownBarHeight,
                   state->attackCooldownTimer, state->attackCooldown, GREEN);

    DrawText("PARRY", hudX, hudY + hudGap, 30, SKYBLUE);
    DrawCooldownBar(hudX, hudY + 35 + hudGap, cooldownBarWidth, cooldownBarHeight,
                   state->parryCooldownTimer, state->parryCooldown, SKYBLUE);

    DrawText("DASH", hudX, hudY + hudGap * 2, 30, ORANGE);
    DrawCooldownBar(hudX, hudY + 35 + hudGap * 2, cooldownBarWidth, cooldownBarHeight,
                   state->dashCooldownTimer, state->dashCooldown, ORANGE);

    // Wave info
    DrawText(TextFormat("Wave: %d", state->currentWave), GetScreenWidth() - 200, 50, 30, WHITE);
    if (state->waveStarting) {
        DrawText(TextFormat("Wave %d em:%.1f", state->currentWave, state->waveStartTimer),
                GetScreenWidth() - 235, 75, 30, WHITE);
    }
}

void DrawMenuScreen(const GameState *state, int screenWidth, int screenHeight) {
    DrawText("Aperte [Enter] para iniciar", screenWidth / 4, screenHeight / 2, 50, WHITE);
}

void DrawGameOverScreen(const GameState *state, int screenWidth, int screenHeight) {
    if (state->death == 1) {
        DrawText("Voce Venceu :)", screenWidth / 4, screenHeight / 2, 50, WHITE);
    }
    else if (state->death == 2) {
        DrawText("Voce Perdeu :(", screenWidth / 4, screenHeight / 2, 50, WHITE);
    }
    DrawText("Aperte [Enter] para retornar ao menu", screenWidth / 4, screenHeight / 2 + 60, 30, WHITE);
}

void DrawUpgradeScreen(const GameState *state, int screenWidth, int screenHeight) {
    DrawText("Escolha um upgrade:", screenWidth / 4, screenHeight / 4, 40, WHITE);
    for (int i = 0; i < 3; i++) {
        DrawText(TextFormat("%d - %s", i + 1, state->upgrades[i].name),
                 screenWidth / 4, screenHeight / 3 + i * 50, 30, GREEN);
    }
}

void DrawGameWorld(const GameState *state) {
    // Platforms
    DrawRectangleRec(state->platform, WHITE);
    DrawRectangleRec(state->miniPlatform, WHITE);
    if (state->currentWave >= 3) {
        DrawRectangleRec(state->platformLeft, WHITE);
        DrawRectangleRec(state->platformRight, WHITE);
    }

    // Player
    if (state->playerInvincibleTimer > 0) {
        if (((int)(state->playerInvincibleTimer * 10) % 2) == 0) {
            DrawRectangleRec(state->player, PURPLE);
        }
    }
    else {
        DrawRectangleRec(state->player, PURPLE);
    }

    // Enemy
    if (state->enemy.alive) DrawRectangleRec(state->enemy.hitbox, YELLOW);

    // Attack
    if (state->isAttacking) DrawRectangleRec(state->attack, RED);

    // Parry
    if (state->isParrying) {
        DrawCircleLines(state->player.x + state->player.width / 2,
                       state->player.y + state->player.height / 2,
                       state->player.width / 2 + 10, SKYBLUE);
    }
}
