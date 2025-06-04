#include "ui_system.h"
#include "game_state.h"
#include "raylib.h"
#include <stdio.h>

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

    // Enemy HP bars
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!state->enemies[i].alive) continue;

        int enemyHpMax = 3;
        if (state->enemies[i].type == ENEMY_TANK) enemyHpMax = 10;

        int enemyBarWidth = (int)(state->enemies[i].hitbox.width * ((float)state->enemies[i].hp / enemyHpMax));
        DrawRectangle((int)state->enemies[i].hitbox.x, (int)(state->enemies[i].hitbox.y + state->enemies[i].hitbox.height + 5),
                     enemyBarWidth, 5, RED);
        DrawRectangleLines((int)state->enemies[i].hitbox.x, (int)(state->enemies[i].hitbox.y + state->enemies[i].hitbox.height + 5),
                          (int)state->enemies[i].hitbox.width, 5, BLACK);
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

    // Draw speed effect timer
    if (state->playerStatus.speedTimer > 0) {
        DrawText(TextFormat("SPEED BOOST: %.1f", state->playerStatus.speedTimer),
                GetScreenWidth() - 250, 100, 20, YELLOW);
    }
}

void DrawMenuScreen(const GameState *state, int screenWidth, int screenHeight) {
    DrawText("Inkolor: Platform Fighter", screenWidth/2 - 150, screenHeight/4, 50, WHITE);
    DrawText("Aperte [Enter] para iniciar", screenWidth/3, screenHeight/2, 40, WHITE);

    // Show high scores
    if (state->scoreCount > 0) {
        DrawText("Melhores Pontuacoes:", screenWidth/3, screenHeight/2 + 100, 30, GOLD);
        for (int i = 0; i < state->scoreCount && i < 5; i++) {
            DrawText(TextFormat("%s: Wave %d",
                    state->highScores[i].name,
                    state->highScores[i].wave),
                    screenWidth/3, screenHeight/2 + 140 + i * 30, 25, WHITE);
        }
    }
}

void DrawGameOverScreen(const GameState *state, int screenWidth, int screenHeight) {
    if (state->death == 1) {
        DrawText("Voce Venceu :)", screenWidth/3, screenHeight/3, 50, GREEN);
    }
    else if (state->death == 2) {
        DrawText("Voce Perdeu :(", screenWidth/3, screenHeight/3, 50, RED);
    }

    DrawText(TextFormat("Wave Alcancada: %d", state->currentWave),
             screenWidth/3, screenHeight/2, 40, WHITE);

    if (state->nameInputActive) {
        DrawText("Digite seu nome:", screenWidth/3, screenHeight/2 + 100, 30, WHITE);
        DrawText(state->playerName, screenWidth/3, screenHeight/2 + 140, 30, YELLOW);
        DrawText("Aperte [Enter] quando terminar", screenWidth/3, screenHeight/2 + 180, 25, WHITE);
    }
    else {
        DrawText("Aperte [Enter] para retornar ao menu", screenWidth/3, screenHeight/2 + 100, 30, WHITE);

        // Show high scores
        DrawText("High Scores:", screenWidth/3, screenHeight/2 + 150, 30, GOLD);
        for (int i = 0; i < state->scoreCount && i < 5; i++) {
            DrawText(TextFormat("%s: Wave %d",
                    state->highScores[i].name,
                    state->highScores[i].wave),
                    screenWidth/3, screenHeight/2 + 190 + i * 30, 25, WHITE);
        }
    }
}

void DrawUpgradeScreen(const GameState *state, int screenWidth, int screenHeight) {
    DrawText("Escolha um upgrade:", screenWidth/3, screenHeight/4, 40, WHITE);
    for (int i = 0; i < 3; i++) {
        DrawText(TextFormat("%d - %s", i + 1, state->upgrades[i].name),
                 screenWidth/3, screenHeight/3 + i * 50, 30, GREEN);
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

    // Enemies
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!state->enemies[i].alive) continue;

        Color enemyColor = YELLOW;
        if (state->enemies[i].type == ENEMY_RANGED) enemyColor = BLUE;
        else if (state->enemies[i].type == ENEMY_TANK) enemyColor = RED;

        DrawRectangleRec(state->enemies[i].hitbox, enemyColor);
    }

    // Projectiles
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (state->projectiles[i].active) {
            Color color = state->projectiles[i].deflected ? GREEN : BLUE;
            DrawCircle(state->projectiles[i].x, state->projectiles[i].y, 5, color);
        }
    }

    // Items
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!state->items[i].active) continue;

        if (state->items[i].type == ITEM_HEALTH) {
            DrawRectangleRec(state->items[i].hitbox, GREEN);
            DrawText("HP", state->items[i].hitbox.x, state->items[i].hitbox.y, 10, BLACK);
        } else if (state->items[i].type == ITEM_SPEED) {
            DrawRectangleRec(state->items[i].hitbox, YELLOW);
            DrawText("SPD", state->items[i].hitbox.x, state->items[i].hitbox.y, 10, BLACK);
        }
    }

    // Attack
    if (state->isAttacking) DrawRectangleRec(state->attack, RED);

    // Parry
    if (state->isParrying) {
        DrawCircleLines(state->player.x + state->player.width / 2,
                       state->player.y + state->player.height / 2,
                       state->player.width / 2 + 10, SKYBLUE);
    }
}
