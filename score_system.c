// score_system.c
#include "score_system.h"
#include "game_state.h"
#include <stdio.h>
#include <string.h>

void SaveHighScore(const char* name, int wave) {
    FILE *file = fopen("scores.dat", "ab");
    if (file) {
        HighScore score;
        strncpy(score.name, name, sizeof(score.name) - 1);
        score.name[sizeof(score.name) - 1] = '\0';
        score.wave = wave;
        fwrite(&score, sizeof(HighScore), 1, file);
        fclose(file);
    }
}

void LoadHighScores(GameState *state) {
    FILE *file = fopen("scores.dat", "rb");
    if (!file) return;

    state->scoreCount = 0;
    while (state->scoreCount < MAX_SCORES &&
           fread(&state->highScores[state->scoreCount], sizeof(HighScore), 1, file)) {
        state->scoreCount++;
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
