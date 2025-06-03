// game_state.h
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"

typedef struct {
    int hp;
    int atk;
    int speed;
} PlayerStatus;

typedef struct {
    int hp;
    int atk;
    int speed;
    Rectangle hitbox;
    bool alive;
    float vel_y;
    float hitCooldownTimer;
} Enemy;

typedef struct {
    const char* name;
    void (*apply)(void);
} Upgrade;

typedef struct {
    // Player
    Rectangle player;
    PlayerStatus playerStatus;
    float vel_y;
    bool isonGround;
    float playerInvincibleTimer;
    char lastMoveDirection;
    float attackWidth;
    float attackHeight;

    // Attack
    Rectangle attack;
    float attackCooldown;
    float attackCooldownTimer;
    float attackDuration;
    float attackDurationTimer;
    bool isAttacking;
    char attackDirection;

    // Parry
    float parryDuration;
    float parryTimer;
    bool isParrying;
    float parryCooldown;
    float parryCooldownTimer;

    // Dash
    float dashCooldown;
    float dashCooldownTimer;
    float dashSpeed;
    float dashDuration;
    float dashDurationTimer;
    bool isDashing;
    char dashDirection;

    // Enemy
    Enemy enemy;

    // Platforms
    Rectangle platform;
    Rectangle miniPlatform;
    Rectangle platformLeft;
    Rectangle platformRight;

    // Game flow
    bool started;
    bool finished;
    int death;

    // Wave system
    int currentWave;
    int enemiesDefeated;
    int enemiesPerWave;
    float waveStartTimer;
    bool waveStarting;

    // Upgrade
    bool onUpgradeScreen;
    int selectedUpgrade;
    bool upgradePending;
    Upgrade upgrades[3];
} GameState;

void InitializeGameState(GameState *state, int screenWidth, int screenHeight);

#endif
