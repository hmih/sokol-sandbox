#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdint.h>
#include <stdio.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;

const int PLAYER = 0;
const int COMPUTER = 1;

const int BAR_WIDTH = 90;
const int BAR_HEIGHT = 15;
const int BAR_MOVE_SPEED = 10;
const int BAR_HOVER_OFFSET = 15;
const int BAR_BOUND_LEFT = 0;
const int BAR_BOUND_RIGHT = SCREEN_WIDTH - BAR_WIDTH;
const int BAR_BOUND_TOP = BAR_HOVER_OFFSET + BAR_HEIGHT; // 0, 0 is top left
const int BAR_BOUND_BOT = SCREEN_HEIGHT - BAR_BOUND_TOP;
const float BAR_INITIAL_X = (float)SCREEN_WIDTH / 2 - (float)BAR_WIDTH / 2;

const int BALL_RADIUS = 10;
const int BALL_SPEED = 5;
const int BALL_BOUND_TOP = BAR_BOUND_TOP + BALL_RADIUS;
const int BALL_BOUND_BOT = BAR_BOUND_BOT - BALL_RADIUS;
const int BALL_BOUND_LEFT = BALL_RADIUS;
const int BALL_BOUND_RIGHT = SCREEN_WIDTH - BALL_RADIUS;
const float BALL_INITIAL_X = (float)SCREEN_WIDTH / 2;
const float BALL_INITIAL_Y = (float)SCREEN_HEIGHT / 2;

const Vector2 REFL_X = {.x = -1, .y = 1};
const Vector2 REFL_Y = {.x = 1, .y = -1};

int main(void);

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "pong");
  int points[2] = {0, 0};

  SetTargetFPS(60);
  Camera2D camera = {0};
  camera.target = Vector2Zero();
  camera.offset = Vector2Zero();
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  int init_dir_x_sign;
  int init_dir_y_sign;
  int init_dir_x;
  const int init_dir_y = 1;
  Vector2 ball_vel;
  Vector2 ball_pos;
  Vector2 com_vel;
  Vector2 com_pos;
  Vector2 plr_vel;
  Vector2 plr_pos;

init:
  init_dir_x_sign = GetRandomValue(1, 2) % 2 == 0;
  init_dir_y_sign = GetRandomValue(1, 2) % 2 == 0;
  init_dir_x = GetRandomValue(0, 1);
  ball_pos = (Vector2){BALL_INITIAL_X, BALL_INITIAL_Y};
  ball_vel = (Vector2){.x = init_dir_x_sign ? init_dir_x : -init_dir_x,
                       .y = init_dir_y_sign ? init_dir_y : -init_dir_y};
  com_vel = (Vector2){.x = ((float)BAR_MOVE_SPEED) / 2, .y = 0};
  com_pos = (Vector2){BAR_INITIAL_X, BAR_HOVER_OFFSET};
  plr_vel = (Vector2){.x = BAR_MOVE_SPEED, .y = 0};
  plr_pos = (Vector2){BAR_INITIAL_X, BAR_BOUND_BOT};

  // set initial ball velocity
  ball_vel = Vector2Scale(ball_vel, BALL_SPEED);

  while (!WindowShouldClose()) {

    if (ball_pos.x < (com_pos.x + (float)BAR_WIDTH / 2)) {
      com_pos = Vector2Add(com_pos, Vector2Negate(com_vel));
    } else if (ball_pos.x > (com_pos.x - (float)BAR_WIDTH / 2)) {
      com_pos = Vector2Add(com_pos, com_vel);
    }
    com_pos.x = Clamp(com_pos.x, BAR_BOUND_LEFT, BAR_BOUND_RIGHT);

    if (IsKeyDown(KEY_D)) {
      plr_pos = Vector2Add(plr_pos, Vector2Negate(plr_vel));
    } else if (IsKeyDown(KEY_F)) {
      plr_pos = Vector2Add(plr_pos, plr_vel);
    }
    plr_pos.x = Clamp(plr_pos.x, BAR_BOUND_LEFT, BAR_BOUND_RIGHT);

    if (ball_pos.y == BALL_BOUND_TOP) {
      if (com_pos.x <= ball_pos.x && (com_pos.x + BAR_WIDTH) >= ball_pos.x) {
        ball_vel = Vector2Multiply(ball_vel, REFL_Y);
      } else {
        points[PLAYER] += 1;
        goto init;
      }
    }

    if (ball_pos.y == BALL_BOUND_BOT) {
      if (plr_pos.x <= ball_pos.x && (plr_pos.x + BAR_WIDTH) >= ball_pos.x) {
        ball_vel = Vector2Multiply(ball_vel, REFL_Y);
      } else {
        points[COMPUTER] += 1;
        goto init;
      }
    }

    if (ball_pos.x == BALL_BOUND_LEFT || ball_pos.x == BALL_BOUND_RIGHT) {
      ball_vel = Vector2Multiply(ball_vel, REFL_X);
    }

    ball_pos = Vector2Add(ball_pos, ball_vel);
    ball_pos = (Vector2){
        .x = Clamp(ball_pos.x, BALL_BOUND_LEFT, BALL_BOUND_RIGHT),
        .y = Clamp(ball_pos.y, BALL_BOUND_TOP, BALL_BOUND_BOT),
    };

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);

    char score_computer[2];
    snprintf(score_computer, 2, "%d", points[COMPUTER]);
    DrawText(score_computer, 30, BAR_BOUND_TOP + 30, 20, BLACK);

    char score_player[2];
    snprintf(score_player, 2, "%d", points[PLAYER]);
    DrawText(score_player, 30, BAR_BOUND_BOT - 30, 20, BLACK);

    Rectangle rect_player = {plr_pos.x, plr_pos.y, BAR_WIDTH, BAR_HEIGHT};
    DrawRectangleRec(rect_player, RED);
    Rectangle rect_computer = {com_pos.x, com_pos.y, BAR_WIDTH, BAR_HEIGHT};
    DrawRectangleRec(rect_computer, BLUE);

    DrawCircle(ball_pos.x, ball_pos.y, BALL_RADIUS, BLACK);

    EndMode2D();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
