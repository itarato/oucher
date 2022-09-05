#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

#include "debug.h"
#include "defs.h"
#include "map.h"
#include "obstacle.h"
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

  // Write lines.
  file << map.lines.size() << endl;
  for (const auto& line : map.lines) {
    file << line.a.x << ' ' << line.a.y << ' ' << line.b.x << ' ' << line.b.y
         << endl;
  }

  // Write obstacles.
  file << map.obstacles.size() << endl;
  for (const auto& obstacle : map.obstacles) {
    file << (int)obstacle.type << ' ' << (int)obstacle.pos.x << ' '
         << (int)obstacle.pos.y << endl;
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

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Oucher editor");

  /****************************************************************************
   * Data
   ***************************************************************************/

  const char* mapFileName = argv[1];

  Vector2 currentMousePos{};

  int currentX{};
  int currentY{};
  int currentGridX{};
  int currentGridY{};

  int offset{0};

  Map map(mapFileName);

  optional<IntVector2> lineStart = nullopt;

  optional<int> previousMouseX = nullopt;

  int selectedObstacle{0};

  while (!WindowShouldClose()) {
    /**************************************************************************
     * Update
     *************************************************************************/

    // Set current cursor.
    currentMousePos = GetMousePosition();
    currentX = GetMouseX() + offset;
    currentY = GetMouseY();
    currentGridX =
        (currentX + (GRID_SIZE / 2)) - (currentX + (GRID_SIZE / 2)) % GRID_SIZE;
    currentGridY =
        (currentY + (GRID_SIZE / 2)) - (currentY + (GRID_SIZE / 2)) % GRID_SIZE;
    IntVector2 currentCoord = IntVector2{currentGridX, currentGridY};

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
        return line.a.x == currentGridX && line.a.y == currentGridY;
      });
    }

    // Obstacle selection.
    int pressedKey = GetKeyPressed();
    if (pressedKey >= KEY_ZERO &&
        pressedKey < (int)(KEY_ZERO + obstacleFramePreset.size())) {
      selectedObstacle = pressedKey - KEY_ZERO;
    }

    // Obstacle creation.
    if (IsKeyDown(selectedObstacle + KEY_ZERO) && IsMouseButtonPressed(2)) {
      map.obstacles.emplace_back((ObstacleType)selectedObstacle,
                                 dx(GetMousePosition(), offset));
    }

    // Obstacle deletion.
    if (IsKeyPressed(KEY_D)) {
      erase_if(map.obstacles, [&](const auto& obstacle) {
        return CheckCollisionPointRec(currentMousePos, obstacle.frame());
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
      if (line.a.x == currentGridX && line.a.y == currentGridY) lineColor = RED;
      DrawLineEx(dx(line.a.v2(), -offset), dx(line.b.v2(), -offset), 1,
                 lineColor);
    }

    // Recorded obstacles;
    for (auto& obstacle : map.obstacles) {
      Color obstacleColor{BROWN};
      Rectangle obstacleAbsFrame = dx(obstacle.frame(), -offset);
      if (CheckCollisionPointRec(currentMousePos, obstacleAbsFrame))
        obstacleColor = RED;
      DrawRectangleRec(obstacleAbsFrame, obstacleColor);
    }

    // Current point cross.
    DrawLineEx(
        Vector2{(float)currentGridX - offset,
                (float)(currentGridY - GRID_SIZE)},
        Vector2{(float)currentGridX - offset, float(currentGridY + GRID_SIZE)},
        3, DARKBROWN);
    DrawLineEx(Vector2{(float)(currentGridX - GRID_SIZE - offset),
                       (float)(currentGridY)},
               Vector2{(float)(currentGridX + GRID_SIZE - offset),
                       (float)(currentGridY)},
               3, DARKBROWN);

    // Current line.
    if (lineStart.has_value()) {
      DrawLineEx(dx(lineStart.value().v2(), -offset),
                 dx(currentCoord.v2(), -offset), 3, DARKBLUE);
    }

    // Current obstacle.
    if (IsKeyDown(selectedObstacle + KEY_ZERO)) {
      DrawRectangleRec(
          Rectangle{(float)currentX - offset, (float)currentY,
                    (float)obstacleFramePreset[selectedObstacle].x,
                    (float)obstacleFramePreset[selectedObstacle].y},
          MAGENTA);
    }

    // HUD.
    Text::build(TextFormat("Map: %s | Pos: %dx:%dy | XOffset: %d", mapFileName,
                           currentGridX, currentGridY, offset))
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
