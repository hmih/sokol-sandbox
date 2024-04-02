#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdint.h>
#include <stdio.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;

const int BAR_WIDTH = 90;
const int BAR_HEIGHT = 15;
const int MOVE_SPEED = 10;
const int BAR_LEFT_BOUND = 0;
const int BAR_RIGHT_BOUND = SCREEN_WIDTH - BAR_WIDTH;

const int HOVER_OFFSET = 15;
const int FLOOR_BOUND = HOVER_OFFSET + BAR_HEIGHT;
const int CEIL_BOUND = SCREEN_HEIGHT - FLOOR_BOUND;

const int BALL_RADIUS = 10;
const int BALL_SPEED = 5;
const int BALL_FLOOR_BOUND = FLOOR_BOUND + BALL_RADIUS;
const int BALL_CEIL_BOUND = CEIL_BOUND - BALL_RADIUS;
const int BALL_LEFT_SIDE_BOUND = BALL_RADIUS;
const int BALL_RIGHT_SIDE_BOUND = SCREEN_WIDTH - BALL_RADIUS;
const float BALL_INITIAL_X = (float)SCREEN_WIDTH / 2;
const float BALL_INITIAL_Y = (float)SCREEN_HEIGHT / 2;

int main(void);

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "pong");
  const int initial_y = GetRandomValue(-1, 1);
  const int initial_x = GetRandomValue(-2, 2);

  Vector2 move_computer = {.x = ((float)MOVE_SPEED) / 2, .y = 0};
  Vector2 move_player = {.x = MOVE_SPEED, .y = 0};
  Vector2 ball = {BALL_INITIAL_X, BALL_INITIAL_Y};
  Vector2 direction = {.y = initial_y, .x = initial_x};
  Vector2 computer = {30, HOVER_OFFSET};
  Vector2 player = {30, CEIL_BOUND};

  SetTargetFPS(60);
  Camera2D camera = {0};
  camera.target = Vector2Zero();
  camera.offset = Vector2Zero();
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  while (!WindowShouldClose()) {

    ball = Vector2Add(ball, direction);
    ball = (Vector2){
        .x = Clamp(ball.x, BALL_LEFT_SIDE_BOUND, BALL_RIGHT_SIDE_BOUND),
        .y = Clamp(ball.y, BALL_FLOOR_BOUND, BALL_CEIL_BOUND)};

    if (ball.x < (computer.x + (float)BAR_WIDTH / 2)) {
      computer = Vector2Add(computer, Vector2Negate(move_computer));
    } else if (ball.x > (computer.x - (float)BAR_WIDTH / 2)) {
      computer = Vector2Add(computer, move_computer);
    }

    if (IsKeyDown(KEY_D)) {
      player = Vector2Add(player, Vector2Negate(move_player));
    } else if (IsKeyDown(KEY_F)) {
      player = Vector2Add(player, move_player);
    }

    if (ball.y == BALL_FLOOR_BOUND) {
      if (computer.x <= ball.x && (computer.x + BAR_WIDTH) >= ball.x) {
        printf("hit computer!\n");
        float a = atan2f(ball.y, ball.x);
        printf("collision angle: %.3f\n", a);
        direction = Vector2Rotate(direction, a);
      }
    }

    if (ball.y == BALL_CEIL_BOUND) {
      if (player.x <= ball.x && (player.x + BAR_WIDTH) >= ball.x) {
        printf("hit player!\n");
        float a = atan2f(ball.y, ball.x);
        printf("collision angle: %.3f\n", a);
        direction = Vector2Rotate(direction, a);
      }
    }

    if (ball.x == BALL_LEFT_SIDE_BOUND || ball.x == BALL_RIGHT_SIDE_BOUND) {
      printf("hit wall!\n");
      float a = atan2f(ball.y, ball.x);
      printf("collision angle: %.3f\n", a);
      direction = Vector2Rotate(direction, a);
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);

    Rectangle rect_player = {player.x, player.y, BAR_WIDTH, BAR_HEIGHT};
    DrawRectangleRec(rect_player, RED);
    Rectangle rect_computer = {computer.x, computer.y, BAR_WIDTH, BAR_HEIGHT};
    DrawRectangleRec(rect_computer, BLUE);
    DrawCircle(ball.x, ball.y, BALL_RADIUS, BLACK);

    EndMode2D();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
