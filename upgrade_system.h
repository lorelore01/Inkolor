#ifndef UPGRADE_SYSTEM_H
#define UPGRADE_SYSTEM_H

#include "game_state.h"

void ApplyUpgrade(GameState *state);
void upgradeAtk(GameState *state);
void upgradeRange(GameState *state);
void upgradeHp(GameState *state);

#endif
