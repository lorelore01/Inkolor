#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define GRAVITY 0.5f
#define JUMP_FORCE -10.0f
#define GROUND_Y 500.0f

typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    float width;
    float height;
    bool isJumping;
} Player;

int main() {
    InitWindow(600, 600, "Inkolor");
    SetTargetFPS(60);

    Player player = { 0 };
    player.position = (Vector2){ 300, GROUND_Y };
    player.velocity = (Vector2){ 0, 0 };
    player.width = 50;
    player.height = 50;
    player.isJumping = false;

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_RIGHT)) player.position.x += 5;
        if (IsKeyDown(KEY_LEFT)) player.position.x -= 5;

        if (IsKeyPressed(KEY_SPACE) && !player.isJumping) {
            player.velocity.y = JUMP_FORCE;
            player.isJumping = true;
        }

        player.velocity.y += GRAVITY;
        player.position.y += player.velocity.y;

        if (player.position.y >= GROUND_Y) {
            player.position.y = GROUND_Y;
            player.velocity.y = 0;
            player.isJumping = false;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangleV(player.position, (Vector2){player.width, player.height}, BLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// MODULARIZAR DEPOIS!!
