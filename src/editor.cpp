#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

#include "debug.h"
#include "map.h"
#include "raylib.h"
#include "text.h"
#include "util.h"

using namespace std;

#define GRID_SIZE 25

/******************************************************************************
 * Types
 *****************************************************************************/

void writeToFile(Map& map, const char* fileName) {
  ofstream file;
  file.open(fileName, ofstream::out | ofstream::in | ofstream::trunc);

  if (!file.is_open()) {
    ERROR("Cannot write to file");
    return;
  }

  file << map.lines.size() << endl;
  for (auto& line : map.lines) {
    file << line.a.x << ' ' << line.a.y << ' ' << line.b.x << ' ' << line.b.y
         << endl;
  }

  file.close();

  LOG("Map saved");
}

int main(int argc, char** argv) {
  LOG("Editor started");

  if (argc != 2) {
    ERROR("Missing map file name. Use: editor FILENAME");
    exit(EXIT_FAILURE);
  }

  InitWindow(2000, 500, "Oucher editor");

  /****************************************************************************
   * Data
   ***************************************************************************/

  const char* mapFileName = argv[1];

  int currentX{};
  int currentY{};

  int offset{0};

  Map map(mapFileName);

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

    // Line deletion.
    if (IsKeyPressed(KEY_D)) {
      erase_if(map.lines, [&](Line& line) {
        return line.a.x == currentX && line.a.y == currentY;
      });
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
      Color lineColor{BLACK};
      if (line.a.x == currentX && line.a.y == currentY) lineColor = RED;
      DrawLineEx(dx(line.a.v2(), -offset), dx(line.b.v2(), -offset), 3,
                 lineColor);
    }

    // Current point cross.
    DrawLineEx(Vector2{(float)currentX - offset, (float)(currentY - GRID_SIZE)},
               Vector2{(float)currentX - offset, float(currentY + GRID_SIZE)},
               3, DARKBROWN);
    DrawLineEx(
        Vector2{(float)(currentX - GRID_SIZE - offset), (float)(currentY)},
        Vector2{(float)(currentX + GRID_SIZE - offset), (float)(currentY)}, 3,
        DARKBROWN);

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

  LOG("Editor exited");

  return EXIT_SUCCESS;
}