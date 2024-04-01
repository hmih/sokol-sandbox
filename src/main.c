#include "raylib.h"
#include "stdint.h"

int main(void);

const int PLAYER = 0;
const int COMPUTER = 1;
const int PLAYERS = 2;

const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 450;

const int RECT_WIDTH = 90;
const int RECT_HEIGHT = 15;
const int HOVER_OFFSET = 15;
const int MOVE_OFFSET = 10;
const int RECT_RIGHT_BOUND = SCREEN_WIDTH - RECT_WIDTH;
const int RECT_LEFT_BOUND = 0;
const int BALL_RADIUS = 10;

float clamp(float min, float curr, float max) {
    bool floor = curr > min;
    bool ceil = curr < max;
    return ceil && floor ? curr : (floor ? ceil : floor);
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "pong");

    struct Vector2 ball = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
    struct Vector2 computer = {30, HOVER_OFFSET};
    struct Vector2 player = {30, SCREEN_HEIGHT - (RECT_HEIGHT + HOVER_OFFSET)};

    SetTargetFPS(60);
    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_D)) {
            const float x = player.x - MOVE_OFFSET;
            player.x = x > RECT_LEFT_BOUND ? x : RECT_LEFT_BOUND;
        }
        else if (IsKeyDown(KEY_F)) {
            const float x = player.x + MOVE_OFFSET;
            player.x = x < RECT_RIGHT_BOUND ? x : RECT_RIGHT_BOUND;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                Rectangle playerRect = { player.x, player.y, RECT_WIDTH, RECT_HEIGHT };
                DrawRectangleRec(playerRect, RED);
                Rectangle computerRect = { computer.x, computer.y, RECT_WIDTH, RECT_HEIGHT };
                DrawRectangleRec(computerRect, BLUE);
                DrawCircle(ball.x, ball.y, BALL_RADIUS, BLACK);
            EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
