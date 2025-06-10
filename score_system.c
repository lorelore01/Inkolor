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
