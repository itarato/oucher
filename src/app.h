#pragma once

#include "player.h"
#include "raylib.h"
#include "util.h"

using namespace std;

struct App {
  Map map{2048,
          512,
          {
              // Lines:
              {{0, 100}, {500, 300}},
              {{500, 300}, {1024, 200}},
              {{1024, 200}, {1500, 500}},
              {{1500, 500}, {2048, 400}},
          }};
  Player player{};
  int offset{0};

  App() = default;

  ~App() { ShowCursor(); }

  void init() {
    InitWindow(1024, 512, "Oucher");
    SetTargetFPS(60);
    HideCursor();
  }

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
    player.update();

    auto dy = deltaYPointToLineList(player.pos, map.lines);
    if (dy != INFINITY) {
      if (dy < 0.0 && dy > PLAYER_LIFT_FROM_BELOW_TRESHOLD) {
        player.pos.y += dy;
        player.v.y = 0.0f;
      }
    }
  }

  void draw() {
    for (auto& line : map.lines) {
      line.draw(xOffset());
    }

    player.draw(xOffset());

    DrawFPS(4, 4);
  }

  int xOffset() const {
    // FIXME: Hardcoded.
    if (map.w < GetScreenWidth()) return 0;
    if (player.pos.x < 200) return 0;

    return min((int)player.pos.x - 200, map.w - GetScreenWidth());
  }
};
