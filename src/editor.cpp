#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

#include "map.h"
#include "raylib.h"
#include "text.h"
#include "util.h"

using namespace std;

#define GRID_SIZE 100

/******************************************************************************
 * Types
 *****************************************************************************/

void writeToFile(Map& map, const char* fileName) {
  ofstream file;
  file.open(fileName, ofstream::out | ofstream::in | ofstream::trunc);

  if (!file.is_open()) {
    cerr << "Cannot write to file\n";
    return;
  }

  file << map.lines.size() << endl;
  for (auto& line : map.lines) {
    file << line.a.x << ' ' << line.a.y << ' ' << line.b.x << ' ' << line.b.y
         << endl;
  }

  file.close();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "Missing map file name. Use: editor FILENAME\n";
    exit(EXIT_FAILURE);
  }

  InitWindow(2000, 512, "Oucher editor");

  /****************************************************************************
   * Data
   ***************************************************************************/

  const char* mapFileName = argv[1];

  int currentX{};
  int currentY{};

  int offset{0};

  Map map{};

  optional<IntVector2> lineStart = nullopt;

  optional<int> previousMouseX = nullopt;

  while (!WindowShouldClose()) {
    /**************************************************************************
     * Update
     *************************************************************************/

    // Set current cursor.
    currentX = ((GetMouseX() + offset) + (GRID_SIZE / 2)) -
               ((GetMouseX() + offset) + (GRID_SIZE / 2)) % GRID_SIZE;
    currentY = (GetMouseY() + (GRID_SIZE / 2)) -
               (GetMouseY() + (GRID_SIZE / 2)) % GRID_SIZE;
    IntVector2 currentCoord = IntVector2{currentX, currentY};

    // Line creation.
    if (IsMouseButtonDown(0) && !lineStart.has_value()) {
      lineStart = currentCoord;
    }
    if (IsMouseButtonReleased(0) && lineStart.has_value()) {
      map.lines.emplace_back(lineStart.value(), currentCoord);
      lineStart = nullopt;
    }

    // Horizontal scrolling.
    if (IsMouseButtonDown(1)) {
      if (!previousMouseX.has_value()) {
        previousMouseX = GetMouseX() + offset;
      }

      if (previousMouseX.has_value()) {
        offset += previousMouseX.value() - (GetMouseX() + offset);
        previousMouseX = GetMouseX() + offset;
      }
    } else {
      if (previousMouseX.has_value()) {
        previousMouseX = nullopt;
      }
    }

    // Save.
    if (IsKeyPressed(KEY_S)) writeToFile(map, mapFileName);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    /**************************************************************************
     * Draw
     *************************************************************************/

    // Recorded lines.
    for (auto& line : map.lines) {
      DrawLineEx(dx(line.a.v2(), -offset), dx(line.b.v2(), -offset), 3, BLACK);
    }

    // Current point cross.
    DrawLineEx(Vector2{(float)currentX - offset, (float)(currentY - GRID_SIZE)},
               Vector2{(float)currentX - offset, float(currentY + GRID_SIZE)},
               3, BLACK);
    DrawLineEx(
        Vector2{(float)(currentX - GRID_SIZE - offset), (float)(currentY)},
        Vector2{(float)(currentX + GRID_SIZE - offset), (float)(currentY)}, 3,
        BLACK);

    // Current line.
    if (lineStart.has_value()) {
      DrawLineEx(dx(lineStart.value().v2(), -offset),
                 dx(currentCoord.v2(), -offset), 3, DARKBLUE);
    }

    // HUD.
    Text::build(TextFormat("Map: %s | Pos: %dx:%dy | XOffset: %d", mapFileName,
                           currentX, currentY, offset))
        .toBottomLeft()
        .withBackgroundColor(LIGHTGRAY)
        .withColor(DARKGRAY)
        .draw();

    EndDrawing();
  }

  CloseWindow();

  return EXIT_SUCCESS;
}
