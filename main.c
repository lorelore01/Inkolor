#include "raylib.h"
#include <math.h>

typedef struct {
    int hp;
    int atk;
    int speed;
} PlayerStatus;

PlayerStatus playerStatus = {10, 1, 5};
float attackWidth = 75;

typedef struct {
    int hp;
    int atk;
    int speed;
    Rectangle hitbox;
    bool alive;
    float vel_y;
    float hitCooldownTimer;
} Enemy;

typedef struct Upgrade {
    const char* name;
    void (*apply)(void);
} Upgrade;

void upgradeAtk() {
    playerStatus.atk += 1;
}

void upgradeRange() {
    attackWidth += 25;
}

void upgradeHp() {
    playerStatus.hp += 3;
    if (playerStatus.hp > 10) playerStatus.hp = 10; // limite máximo
}

Upgrade upgrades[3] = {
    {"+1 Dano", upgradeAtk},
    {"+25 de alcance", upgradeRange},
    {"+3 de vida", upgradeHp}
};

void WrapAroundPlayer(Rectangle *player, int screenWidth) {
    if (player->x + player->width < 0) player->x = screenWidth;
    if (player->x > screenWidth) player->x = -player->width;
}

void WrapAroundEnemy(Enemy *enemy, int screenWidth) {
    if (enemy->hitbox.x + enemy->hitbox.width < 0) enemy->hitbox.x = screenWidth;
    if (enemy->hitbox.x > screenWidth) enemy->hitbox.x = -enemy->hitbox.width;
}

void DrawCooldownBar(int x, int y, int width, int height, float cooldownTimer, float cooldownMax, Color color) {
    float percent = 1.0f;
    if (cooldownMax > 0) percent = 1.0f - (cooldownTimer / cooldownMax);
    if (percent < 0) percent = 0;
    if (percent > 1) percent = 1;
    DrawRectangle(x, y, width, height, DARKGRAY);
    DrawRectangle(x, y, (int)(width * percent), height, color);
    DrawRectangleLines(x, y, width, height, BLACK);
}

int main() {
    const int screenWidth = 1600;
    const int screenHeight = 920;
    InitWindow(screenWidth, screenHeight, "Tela");

    Rectangle player = {100, 500, 50, 50};
    Rectangle platform = {50, 820, 1500, 50};
    Rectangle miniPlatform = { screenWidth / 2 - 75, 525, 150, 20 };
    Rectangle platformLeft = { 100, 525, 300, 20 };
    Rectangle platformRight = { 1200, 525, 300, 20 };

    Enemy enemy = {3, 1, 4, {800, 770, 50, 50}, true, 0, 0};

    Rectangle attack = {0, 0, 0, 0};
    float vel_y = 0;
    float gravity = 1;
    float jump_power = -25;
    bool isonGround = false;

    float attackCooldown = 0.5f;
    float attackCooldownTimer = 0;
    float attackDuration = 0.15f;
    float attackDurationTimer = 0;
    bool isAttacking = false;
    char attackDirection = ' ';

    float playerInvincibleTimer = 0.0f;
    float playerInvincibleDuration = 0.75f;

    float attackHeight = 30;

    float parryDuration = 0.5f;
    float parryTimer = 0;
    bool isParrying = false;
    float parryCooldown = 3.0f;
    float parryCooldownTimer = 0;

    float dashCooldown = 1.5f;
    float dashCooldownTimer = 0;
    float dashSpeed = 20;
    float dashDuration = 0.2f;
    float dashDurationTimer = 0;
    bool isDashing = false;
    char dashDirection = ' ';
    char lastMoveDirection = 'D';

    bool started = false;
    bool finished = false;
    int death = 0;

    int currentWave = 1;
    int enemiesDefeated = 0;
    int enemiesPerWave = 1;
    float waveStartTimer = .0f;
    bool waveStarting = true;

    bool onUpgradeScreen = false;
    int selectedUpgrade = -1;
    bool upgradePending = false;

    SetTargetFPS(60);

while (!WindowShouldClose()) {
    float delta = GetFrameTime();

    // Gravidade
    vel_y += gravity;
    player.y += vel_y;
    WrapAroundPlayer(&player, screenWidth);

    isonGround = false;

    if (vel_y >= 0) {
        bool onPlatform = false;

        if (player.y + player.height <= platform.y + 5 &&
            player.y + player.height + vel_y >= platform.y &&
            player.x + player.width > platform.x &&
            player.x < platform.x + platform.width) {
            player.y = platform.y - player.height;
            vel_y = 0;
            isonGround = true;
            onPlatform = true;
        }
        else if (player.y + player.height <= miniPlatform.y + 5 &&
                 player.y + player.height + vel_y >= miniPlatform.y &&
                 player.x + player.width > miniPlatform.x &&
                 player.x < miniPlatform.x + miniPlatform.width) {
            player.y = miniPlatform.y - player.height;
            vel_y = 0;
            isonGround = true;
            onPlatform = true;
        }
        else if (currentWave >= 3) {
            if (player.y + player.height <= platformLeft.y + 5 &&
                player.y + player.height + vel_y >= platformLeft.y &&
                player.x + player.width > platformLeft.x &&
                player.x < platformLeft.x + platformLeft.width) {
                player.y = platformLeft.y - player.height;
                vel_y = 0;
                isonGround = true;
                onPlatform = true;
            }
            else if (player.y + player.height <= platformRight.y + 5 &&
                     player.y + player.height + vel_y >= platformRight.y &&
                     player.x + player.width > platformRight.x &&
                     player.x < platformRight.x + platformRight.width) {
                player.y = platformRight.y - player.height;
                vel_y = 0;
                isonGround = true;
                onPlatform = true;
            }
        }

        if (!onPlatform) isonGround = false;
    }


    // Movimento
    if (!isDashing) {
        if (IsKeyDown(KEY_A)) {
            player.x -= playerStatus.speed;
            lastMoveDirection = 'A';
        }
        if (IsKeyDown(KEY_D)) {
            player.x += playerStatus.speed;
            lastMoveDirection = 'D';
        }
    }

    // Pulo
    if (IsKeyPressed(KEY_SPACE) && isonGround)
        vel_y = jump_power;

    // Queda fora da tela
    if (player.y > screenHeight) {
        playerStatus.hp--;
        player.x = screenWidth / 2 - player.width / 2;
        player.y = platform.y - player.height;
        playerInvincibleTimer = 1;
        vel_y = 0;
    }

    if (playerStatus.hp <= 0) {
        finished = true;
        death = 2;
    }

    // Cooldowns
    attackCooldownTimer -= delta;
    if (attackCooldownTimer < 0) attackCooldownTimer = 0;
    attackDurationTimer -= delta;
    if (attackDurationTimer < 0) attackDurationTimer = 0;
    parryCooldownTimer -= delta;
    if (parryCooldownTimer < 0) parryCooldownTimer = 0;
    parryTimer -= delta;
    if (parryTimer < 0) {
        parryTimer = 0;
        isParrying = false;
    }
    dashCooldownTimer -= delta;
    if (dashCooldownTimer < 0) dashCooldownTimer = 0;

    // Ataques
    if (IsKeyPressed(KEY_J) && attackCooldownTimer <= 0) {
        attack = (Rectangle){player.x - attackWidth, player.y + (player.height / 2 - attackHeight / 2), attackWidth, attackHeight};
        isAttacking = true;
        attackCooldownTimer = attackCooldown;
        attackDurationTimer = attackDuration;
        attackDirection = 'J';
    }
    if (IsKeyPressed(KEY_L) && attackCooldownTimer <= 0) {
        attack = (Rectangle){player.x + player.width, player.y + (player.height / 2 - attackHeight / 2), attackWidth, attackHeight};
        isAttacking = true;
        attackCooldownTimer = attackCooldown;
        attackDurationTimer = attackDuration;
        attackDirection = 'L';
    }
    if (IsKeyPressed(KEY_I) && attackCooldownTimer <= 0) {
        attack = (Rectangle){player.x + (player.width / 2 - attackHeight / 2), player.y - attackWidth, attackHeight, attackWidth};
        isAttacking = true;
        attackCooldownTimer = attackCooldown;
        attackDurationTimer = attackDuration;
        attackDirection = 'I';
    }
    if (IsKeyPressed(KEY_K) && attackCooldownTimer <= 0) {
        attack = (Rectangle){player.x + (player.width / 2 - attackHeight / 2), player.y + player.height, attackHeight, attackWidth};
        isAttacking = true;
        attackCooldownTimer = attackCooldown;
        attackDurationTimer = attackDuration;
        attackDirection = 'K';
    }
    if (attackDurationTimer == 0) isAttacking = false;

    // Dash
    if (IsKeyPressed(KEY_O) && dashCooldownTimer <= 0 && !isDashing) {
        isDashing = true;
        dashDurationTimer = dashDuration;
        dashCooldownTimer = dashCooldown;
        playerInvincibleTimer = dashDuration;

        if (IsKeyDown(KEY_A)) dashDirection = 'A';
        else if (IsKeyDown(KEY_D)) dashDirection = 'D';
        else dashDirection = lastMoveDirection;
    }

    if (isDashing) {
        if (dashDirection == 'A') player.x -= dashSpeed;
        else if (dashDirection == 'D') player.x += dashSpeed;
        WrapAroundPlayer(&player, screenWidth);
        dashDurationTimer -= delta;
        if (dashDurationTimer <= 0) {
            isDashing = false;
        }
    }

    // Parry
    if (IsKeyPressed(KEY_U) && parryCooldownTimer <= 0) {
        isParrying = true;
        parryTimer = parryDuration;
        parryCooldownTimer = parryCooldown;
        playerInvincibleTimer = parryDuration;
    }

    // Inimigo bate no player
    if (enemy.alive && CheckCollisionRecs(player, enemy.hitbox)) {
        if (playerInvincibleTimer <= 0) {
            playerStatus.hp -= enemy.atk;
            playerInvincibleTimer = playerInvincibleDuration;
            if (playerStatus.hp <= 0) {
                finished = true;
                death = 2;
            }
        }
    }

        if (playerInvincibleTimer > 0) playerInvincibleTimer -= delta;
        if (playerInvincibleTimer < 0) playerInvincibleTimer = 0;

        if (enemy.hitCooldownTimer > 0) enemy.hitCooldownTimer -= delta;
        if (enemy.hitCooldownTimer < 0) enemy.hitCooldownTimer = 0;

        if (!started) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Aperte [Enter] para iniciar", screenWidth / 4, screenHeight / 2, 50, WHITE);
            EndDrawing();

            if (IsKeyPressed(KEY_ENTER)) {
                started = true;
                player.x = miniPlatform.x + (miniPlatform.width / 2) - (player.width / 2);
                player.y = miniPlatform.y - player.height;
                vel_y = 0;
            }

            if (IsKeyPressed(KEY_ENTER)) started = true;
        }
        else if (finished) {
            BeginDrawing();
            ClearBackground(BLACK);
            if (death == 1)
                DrawText("Voce Venceu :)", screenWidth / 4, screenHeight / 2, 50, WHITE);
            else if (death == 2)
                DrawText("Voce Perdeu :(", screenWidth / 4, screenHeight / 2, 50, WHITE);

            DrawText("Aperte [Enter] para retornar ao menu", screenWidth / 4, screenHeight / 2 + 60, 30, WHITE);
            EndDrawing();

            if (IsKeyPressed(KEY_ENTER)) {
                started = false;
                finished = false;
                death = 0;

                playerStatus.hp = 10;
                playerStatus.atk = 1;
                playerStatus.speed = 5;
                attackWidth = 75;
                player.x = miniPlatform.x + (miniPlatform.width / 2) - (player.width / 2);
                player.y = miniPlatform.y - player.height;
                vel_y = 0;

                enemy.alive = true;
                enemy.hp = 3;
                enemy.hitbox.x = 800;
                enemy.hitbox.y = 770;
                enemy.vel_y = 0;

                currentWave = 1;
                waveStarting = true;
            }
        }
        else {
            if (waveStarting) {
                waveStartTimer -= delta;
                if (waveStartTimer <= 0.0f) {
                    waveStarting = false;
                    enemy.alive = true;
                    enemy.hp = 3;
                    enemy.hitbox.x = 800;
                    enemy.hitbox.y = 770;
                    enemy.vel_y = 0;
                }
            }
            else {

                // Lógica inimigo
                if (enemy.alive) {
                    enemy.vel_y += gravity;
                    enemy.hitbox.y += enemy.vel_y;

                    if (CheckCollisionRecs(enemy.hitbox, platform)) {
                        enemy.hitbox.y = platform.y - enemy.hitbox.height;
                        enemy.vel_y = 0;
                    }

                    if (enemy.hitbox.x < player.x) enemy.hitbox.x += enemy.speed;
                    else if (enemy.hitbox.x > player.x) enemy.hitbox.x -= enemy.speed;

                    if (player.y + player.height < enemy.hitbox.y && fabsf(player.x - enemy.hitbox.x) < 100 && enemy.vel_y == 0) {
                        enemy.vel_y = jump_power / 1.5f;
                    }

                    WrapAroundEnemy(&enemy, screenWidth);

                    if (CheckCollisionRecs(player, enemy.hitbox)) {
                        if (playerInvincibleTimer <= 0) {
                            playerStatus.hp -= enemy.atk;
                            playerInvincibleTimer = playerInvincibleDuration;
                            if (playerStatus.hp <= 0) {
                                finished = true;
                                death = 2;
                            }
                        }
                    }
                }

                // Ataque colide com inimigo
                if (isAttacking && enemy.alive) {
                    if (CheckCollisionRecs(attack, enemy.hitbox)) {
                        if (attackDirection == 'K') vel_y = jump_power / 2;
                        if (enemy.hitCooldownTimer == 0) {
                            enemy.hp -= playerStatus.atk;
                            enemy.hitCooldownTimer = 0.3f;
                            if (enemy.hp <= 0) {
                                enemy.alive = false;
                            }
                        }
                    }
                }

                // Quando inimigo morre
                if (!enemy.alive) {
                    enemiesDefeated++;
                    if (enemiesDefeated >= enemiesPerWave) {
                        enemiesDefeated = 0;

                        if ((currentWave == 2 || currentWave == 4) && upgradePending) {
                            // Primeira vez, abre upgrade
                            onUpgradeScreen = true;
                        }
                        else {
                            // Avança wave
                            currentWave++;
                            if (currentWave > 5) {
                                finished = true;
                                death = 1;
                            } else {
                                // Se próxima wave for 2 ou 4, marca que upgrade precisa acontecer
                                if (currentWave == 2 || currentWave == 4) {
                                    upgradePending = true;
                                }
                                waveStartTimer = 2.0f;
                                waveStarting = true;
                            }
                        }
                    } else {
                        enemy.alive = true;
                        enemy.hp = 3;
                        enemy.hitbox.x = 800;
                        enemy.hitbox.y = 770;
                        enemy.vel_y = 0;
                    }
                }
            }
            if (onUpgradeScreen) {
                BeginDrawing();
                ClearBackground(BLACK);

                DrawText("Escolha um upgrade:", screenWidth / 4, screenHeight / 4, 40, WHITE);

                for (int i = 0; i < 3; i++) {
                    DrawText(TextFormat("%d - %s", i + 1, upgrades[i].name),
                             screenWidth / 4, screenHeight / 3 + i * 50, 30, GREEN);
                }

                EndDrawing();

                for (int i = 0; i < 3; i++) {
                    if (IsKeyPressed(KEY_ONE + i)) {
                        selectedUpgrade = i;
                        break;
                    }
                }

                    if (selectedUpgrade != -1) {
                        upgrades[selectedUpgrade].apply();
                        selectedUpgrade = -1;
                        onUpgradeScreen = false;
                        upgradePending = false;
                        currentWave++;
                        waveStartTimer = 2.0f;
                        waveStarting = true;
                    }

                    if (upgradePending) {
                        upgradePending = false;
                        waveStartTimer = 2.0f;
                        waveStarting = true;
                    }
                }
            else {
                BeginDrawing();
                ClearBackground(BLACK);
                DrawRectangleRec(platform, WHITE);
                DrawRectangleRec(miniPlatform, WHITE);
                if (currentWave >= 3) {
                    DrawRectangleRec(platformLeft, WHITE);
                    DrawRectangleRec(platformRight, WHITE);
                }

                if (playerInvincibleTimer > 0) {
                    if (((int)(playerInvincibleTimer * 10) % 2) == 0) {
                        DrawRectangleRec(player, PURPLE);
                    }
                }
                else {
                    DrawRectangleRec(player, PURPLE);
                }

                if (enemy.alive) DrawRectangleRec(enemy.hitbox, YELLOW);
                if (isAttacking) DrawRectangleRec(attack, RED);
                if (isParrying) DrawCircleLines(player.x + player.width / 2, player.y + player.height / 2, player.width / 2 + 10, SKYBLUE);

                int barWidth = 200;
                int barHeight = 20;
                int barX = 50;
                int barY = 50;
                DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
                int hpWidth = (playerStatus.hp * barWidth) / 10;
                DrawRectangle(barX, barY, hpWidth, barHeight, RED);
                DrawText(TextFormat("HP: %d / 10", playerStatus.hp), barX, barY - 25, 20, WHITE);

                if (enemy.alive) {
                    int enemyHpMax = 3;
                    int enemyBarWidth = (int)(enemy.hitbox.width * ((float)enemy.hp / enemyHpMax));
                    DrawRectangle((int)enemy.hitbox.x, (int)(enemy.hitbox.y + enemy.hitbox.height + 5), enemyBarWidth, 5, RED);
                    DrawRectangleLines((int)enemy.hitbox.x, (int)(enemy.hitbox.y + enemy.hitbox.height + 5), (int)enemy.hitbox.width, 5, BLACK);
                }

                int hudX = 50;
                int hudY = 80;
                int cooldownBarWidth = 100;
                int cooldownBarHeight = 15;
                int hudGap = 75;
                DrawText("ATK", hudX, hudY, 30, GREEN);
                DrawCooldownBar(hudX, hudY + 35, cooldownBarWidth, cooldownBarHeight, attackCooldownTimer, attackCooldown, GREEN);
                DrawText("PARRY", hudX, hudY + hudGap, 30, SKYBLUE);
                DrawCooldownBar(hudX, hudY + 35 + hudGap, cooldownBarWidth, cooldownBarHeight, parryCooldownTimer, parryCooldown, SKYBLUE);
                DrawText("DASH", hudX, hudY + hudGap * 2, 30, ORANGE);
                DrawCooldownBar(hudX, hudY + 35 + hudGap * 2, cooldownBarWidth, cooldownBarHeight, dashCooldownTimer, dashCooldown, ORANGE);
                DrawText(TextFormat("Wave: %d", currentWave), screenWidth - 200, 50, 30, WHITE);
                if (waveStarting) {
                DrawText(TextFormat("Wave %d em:%.1f", currentWave, waveStartTimer), screenWidth - 235, 75, 30, WHITE);
                }

                EndDrawing();
            }
        }
    }

    CloseWindow();

    return 0;
}
