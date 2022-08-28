#include <optional>
#include <utility>
#include <vector>

#include "raylib.h"

using namespace std;

#define GRID_SIZE 10

struct IntCoord {
  int x;
  int y;

  Vector2 v2() { return Vector2{(float)x, (float)y}; }
};

int main() {
  InitWindow(2000, 512, "Oucher editor");

  int currentX{};
  int currentY{};

  // int offset{0};

  vector<pair<IntCoord, IntCoord>> lines{};

  optional<IntCoord> lineStart = nullopt;

  while (!WindowShouldClose()) {
    currentX = (GetMouseX() + (GRID_SIZE / 2)) -
               (GetMouseX() + (GRID_SIZE / 2)) % GRID_SIZE;
    currentY = (GetMouseY() + (GRID_SIZE / 2)) -
               (GetMouseY() + (GRID_SIZE / 2)) % GRID_SIZE;
    IntCoord currentCoord = IntCoord{currentX, currentY};

    if (IsMouseButtonDown(0) && !lineStart.has_value()) {
      lineStart = currentCoord;
    }
    if (IsMouseButtonReleased(0) && lineStart.has_value()) {
      lines.emplace_back(lineStart.value(), currentCoord);
      lineStart = nullopt;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Recorded lines.
    for (auto& line : lines) {
      DrawLineEx(line.first.v2(), line.second.v2(), 3, BLACK);
    }

    // Current line.
    if (lineStart.has_value()) {
      DrawLineEx(lineStart.value().v2(), currentCoord.v2(), 3, DARKBLUE);
    }

    // Current point cross.
    DrawLineEx(Vector2{(float)currentX, (float)(currentY - GRID_SIZE)},
               Vector2{(float)currentX, float(currentY + GRID_SIZE)}, 3, BLACK);
    DrawLineEx(Vector2{(float)(currentX - GRID_SIZE), (float)(currentY)},
               Vector2{(float)(currentX + GRID_SIZE), (float)(currentY)}, 3,
               BLACK);

    DrawText(TextFormat("%dx:%dy", currentX, currentY), 12,
             GetScreenHeight() - 22, 10, DARKGREEN);

    EndDrawing();
  }

  CloseWindow();
}
