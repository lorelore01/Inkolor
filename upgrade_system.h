#ifndef UPGRADE_SYSTEM_H
#define UPGRADE_SYSTEM_H

// Forward declaration
typedef struct GameState GameState;

void ApplyUpgrade(GameState *state);
void upgradeAtk(GameState *state);
void upgradeRange(GameState *state);
void upgradeHp(GameState *state);

#endif
