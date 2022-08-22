#pragma once

#include "player.h"
#include "raylib.h"
#include "util.h"

using namespace std;

struct App {
  Map map{0,
          1024,
          {
              // Triangles:
              {{0, 100}, {200, 500}, {500, 200}},
              {{500, 200}, {900, 300}, {1024, 100}},
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

    auto dy = Util::deltaYPointToTriangleList(player.pos, map.triangles);
    if (dy != INFINITY) {
      DrawLine(player.pos.x, player.pos.y, player.pos.x, player.pos.y + dy,
               RED);

      if (dy < 0.0 && dy > PLAYER_LIFT_FROM_BELOW_TRESHOLD) {
        player.pos.y += dy;
        player.v.y = 0.0f;
      }
    }
  }

  void draw() {
    for (auto& triangle : map.triangles) {
      DrawTriangleLines(triangle.a.v2(), triangle.b.v2(), triangle.c.v2(),
                        BLACK);
    }

    DrawCircle(GetMouseX(), GetMouseY(), 5.0f, GREEN);

    player.draw();

    DrawFPS(4, 4);
  }
};
