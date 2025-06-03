#include "score_system.h"
#include "game_state.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void SaveHighScore(const char* name, int wave) {
    FILE *file = fopen("scores.txt", "a");
    if (file) {
        fprintf(file, "%s;%d\n", name, wave);
        fclose(file);
    }
}

void LoadHighScores(GameState *state) {
    FILE *file = fopen("scores.txt", "r");
    if (!file) return;

    char line[100];
    state->scoreCount = 0;

    while (fgets(line, sizeof(line), file) && state->scoreCount < MAX_SCORES) {
        char *name = strtok(line, ";");
        char *waveStr = strtok(NULL, ";");

        if (name && waveStr) {
            strncpy(state->highScores[state->scoreCount].name, name, 49);
            state->highScores[state->scoreCount].name[49] = '\0';
            state->highScores[state->scoreCount].wave = atoi(waveStr);
            state->scoreCount++;
        }
    }
    fclose(file);
}

void UpdateProjectiles(GameState *state) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!state->projectiles[i].active) continue;

        // Update position
        state->projectiles[i].x += state->projectiles[i].dx;
        state->projectiles[i].y += state->projectiles[i].dy;

        // Check collision with player
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
        }

        // Check if out of screen
        if (state->projectiles[i].x < 0 || state->projectiles[i].x > GetScreenWidth() ||
            state->projectiles[i].y < 0 || state->projectiles[i].y > GetScreenHeight()) {
            state->projectiles[i].active = false;
        }
    }
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
