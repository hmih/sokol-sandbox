#include <raylib.h>
#include <raymath.h>
#include <stdint.h>
#include <stdio.h>

int main(void);

const int PLAYER = 0;
const int COMPUTER = 1;
const int PLAYERS = 2;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;

const int RECT_WIDTH = 90;
const int RECT_HEIGHT = 15;
const int HOVER_OFFSET = 15;
const int MOVE_SPEED = 10;
const int RECT_RIGHT_BOUND = SCREEN_WIDTH - RECT_WIDTH;
const int RECT_LEFT_BOUND = 0;

const int BALL_RADIUS = 10;
const int BALL_SPEED = 2;

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "pong");
  const int initial_y = GetRandomValue(-1, 1);
  const int initial_x = GetRandomValue(-2, 2);

  Vector2 move_computer = {.x = ((float)MOVE_SPEED) / 2, .y = 0};
  Vector2 move_player = {.x = MOVE_SPEED, .y = 0};
  Vector2 ball = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
  Vector2 direction = {.y = initial_y, .x = initial_x};
  Vector2 computer = {30, HOVER_OFFSET};
  Vector2 player = {30, SCREEN_HEIGHT - (RECT_HEIGHT + HOVER_OFFSET)};

  SetTargetFPS(60);
  Camera2D camera = {0};
  camera.target = (Vector2){0, 0};
  camera.offset = (Vector2){0, 0};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  while (!WindowShouldClose()) {

    ball = Vector2Add(ball, direction);

    if (ball.x < (computer.x + (float)RECT_WIDTH / 2)) {
      computer = Vector2Add(computer, Vector2Negate(move_computer));
    } else if (ball.x > (computer.x - (float)RECT_WIDTH / 2)) {
      computer = Vector2Add(computer, move_computer);
    }

    if (IsKeyDown(KEY_D)) {
      player = Vector2Add(player, Vector2Negate(move_player));
    } else if (IsKeyDown(KEY_F)) {
      player = Vector2Add(player, move_player);
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);

    Rectangle rect_player = {player.x, player.y, RECT_WIDTH, RECT_HEIGHT};
    DrawRectangleRec(rect_player, RED);
    Rectangle rect_computer = {computer.x, computer.y, RECT_WIDTH, RECT_HEIGHT};
    DrawRectangleRec(rect_computer, BLUE);
    DrawCircle(ball.x, ball.y, BALL_RADIUS, BLACK);

    EndMode2D();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
