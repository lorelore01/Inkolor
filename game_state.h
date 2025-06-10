#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"
#include <stddef.h>

#define MAX_ENEMIES 15
#define MAX_PROJECTILES 50
#define MAX_ITEMS 10

// Forward declaration
typedef struct GameState GameState;

typedef enum {
    ENEMY_MELEE,
    ENEMY_RANGED,
    ENEMY_TANK
} EnemyType;

typedef enum {
    ITEM_NONE,
    ITEM_HEALTH,
    ITEM_SPEED
} ItemType;

typedef struct {
    int hp;
    int atk;
    int speed;
    float speedTimer; // Timer for speed(Power-UP)
    int originalSpeed;
} PlayerStatus;

typedef struct Enemy {
    EnemyType type;
    int hp;
    int maxHp; // Adicionado: HP máximo para barra de vida
    int atk;
    int speed;
    Rectangle hitbox;
    bool alive;
    float vel_y;
    float hitCooldownTimer;
    float shootCooldown;
} Enemy;

typedef struct {
    float x, y;
    float dx, dy;
    bool active;
    bool deflected;
} Projectile;

typedef struct {
    ItemType type;
    Rectangle hitbox;
    bool active;
    float lifeTimer;
} Item;

typedef struct {
    const char* name;
    void (*apply)(GameState*);
} Upgrade;

struct GameState {
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

    // Enemies
    Enemy enemies[MAX_ENEMIES];
    int enemiesAlive;

    // Projectiles
    Projectile projectiles[MAX_PROJECTILES];

    // Items
    Item items[MAX_ITEMS];

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
    float waveStartTimer;
    bool waveStarting;

    // Upgrade
    bool onUpgradeScreen;
    int selectedUpgrade;
    bool upgradePending;
    Upgrade upgrades[3];

    // Player name
    char playerName[50];
    bool nameInputActive;
};

void UpdateItems(GameState *state, float delta);
void InitializeGameState(GameState *state, int screenWidth, int screenHeight);

#endif
