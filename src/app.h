#pragma once

#include "map.h"
#include "player.h"
#include "raylib.h"
#include "util.h"

#define APP_SCROLL_START_PADDING 200

using namespace std;

enum class AppState {
  Ready,
  Running,
};

struct App {
  int stage{};

  vector<Map> maps{
      {
          {{0, 400}, {1024, 400}},
      },
      {
          {{0, 100}, {500, 300}, {1024, 200}, {1500, 500}, {2048, 400}},
      },
  };

  Player player{};

  int offset{0};

  AppState state{AppState::Ready};

  App() = default;

  ~App() { ShowCursor(); }

  void init() {
    InitWindow(1024, 512, "Oucher V0.1 pre-alpha dev build");
    SetTargetFPS(60);
    HideCursor();

    stage = 0;

    restart();
  }

  void restart() {}

  void loop() {
    while (!WindowShouldClose()) {
      update();

      BeginDrawing();
      ClearBackground(RAYWHITE);

      draw();

      EndDrawing();
    }
  }

  void update() {
    if (state == AppState::Running) {
      update_game();
    } else if (state == AppState::Ready) {
      if (IsKeyPressed(KEY_ENTER)) {
        state = AppState::Running;
      }
    }
  }

  void update_game() {
    player.update();

    auto dy = maps[stage].deltaYPointToSurface(player.pos);
    if (dy != INFINITY) {
      if (dy < 0.0 && dy > PLAYER_LIFT_FROM_BELOW_TRESHOLD) {
        player.pos.y += dy;
        player.v.y = 0.0f;
      }
    }
  }

  void draw() {
    if (state == AppState::Running) {
      draw_game();
    } else if (state == AppState::Ready) {
      draw_menu();
    }
  }

  void draw_menu() {
    DrawText(TextFormat("Stage [%d] - Press [Enter]", stage), 10, 10, 10,
             DARKGRAY);
  }

  void draw_game() {
    maps[stage].draw(xOffset());
    player.draw(xOffset());

    // DrawFPS(4, 4);
  }

  int xOffset() const {
    if (maps[stage].w < GetScreenWidth()) return 0;
    if (player.pos.x < APP_SCROLL_START_PADDING) return 0;

    return min((int)player.pos.x - APP_SCROLL_START_PADDING,
               maps[stage].w - GetScreenWidth());
  }
};
