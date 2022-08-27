#pragma once

#include "obstacle.h"
#include "player.h"
#include "raylib.h"
#include "util.h"

#define APP_SCROLL_START_PADDING 200

using namespace std;

struct App {
  Map map{
      {{0, 100}, {500, 300}, {1024, 200}, {1500, 500}, {2048, 400}},
  };

  Player player{};

  vector<Obstacle> obstacles{
      Obstacle({400, 300, 60, 80}),
  };

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
    for (auto& line : map.lines) line.draw(xOffset());
    for (auto& obstacle : obstacles) obstacle.draw(xOffset());

    player.draw(xOffset());

    DrawFPS(4, 4);
  }

  int xOffset() const {
    if (map.w < GetScreenWidth()) return 0;
    if (player.pos.x < APP_SCROLL_START_PADDING) return 0;

    return min((int)player.pos.x - APP_SCROLL_START_PADDING,
               map.w - GetScreenWidth());
  }
};
