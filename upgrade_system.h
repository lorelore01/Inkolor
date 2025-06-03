// upgrade_system.h
#ifndef UPGRADE_SYSTEM_H
#define UPGRADE_SYSTEM_H

#include "game_state.h"

// Adicione esta declaração
void SetGlobalState(GameState* state);

void ApplyUpgrade(GameState *state);
void upgradeAtk(void);
void upgradeRange(void);
void upgradeHp(void);

#endif
