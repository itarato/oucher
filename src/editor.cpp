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

  int objectLen{0};
  objectLen += map.obstacles.size();
  objectLen += map.decorations.size();
  objectLen += map.trampolines.size();

  file << objectLen << endl;

  // Write obstacles.
  for (const auto& obstacle : map.obstacles) {
    file << (int)obstacle.type << ' ' << (int)obstacle.pos.x << ' '
         << (int)obstacle.pos.y << endl;
  }

  // Write decorations.
  for (const auto& decoration : map.decorations) {
    file << (int)decoration.type + STATIC_OBJECT_ID_DECORATION_START << ' '
         << (int)decoration.pos.x << ' ' << (int)decoration.pos.y << endl;
  }

  // Write trampolines.
  for (const auto& trampoline : map.trampolines) {
    file << STATIC_OBJECT_ID_TRAMPOLINE << ' ' << (int)trampoline.pos.x << ' '
         << (int)trampoline.pos.y << endl;
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

  int selectedObstacle{-1};
  int selectedDecoration{-1};

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
        previousMouseX = currentX;
      }

      if (previousMouseX.has_value()) {
        offset += previousMouseX.value() - currentX;
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

    if (IsKeyPressed(KEY_X)) {
      selectedDecoration = -1;
      selectedObstacle = -1;
    }

    int pressedKey = GetKeyPressed();

    // Obstacle selection.
    if (IsKeyUp(KEY_LEFT_SHIFT) && pressedKey >= KEY_ZERO &&
        pressedKey < (int)(KEY_ZERO + obstacleFramePreset.size())) {
      selectedObstacle = pressedKey - KEY_ZERO;
      selectedDecoration = -1;
    }

    // Obstacle creation.
    if (selectedObstacle >= 0 && IsMouseButtonPressed(2)) {
      map.obstacles.emplace_back((ObstacleType)selectedObstacle,
                                 dx(GetMousePosition(), offset));
    }

    // Obstacle deletion.
    if (IsKeyPressed(KEY_D)) {
      erase_if(map.obstacles, [&](const auto& obstacle) {
        return CheckCollisionPointRec(currentMousePos, obstacle.frame());
      });
    }

    // Decoration selection.
    if (IsKeyDown(KEY_LEFT_SHIFT) && pressedKey >= KEY_ZERO &&
        pressedKey < (int)(KEY_ZERO + decorationFramePreset.size())) {
      selectedDecoration = pressedKey - KEY_ZERO;
      selectedObstacle = -1;
    }

    // Decoration creation.
    if (selectedDecoration >= 0 && IsMouseButtonPressed(2)) {
      map.decorations.emplace_back((DecorationType)selectedDecoration,
                                   dx(GetMousePosition(), offset));
    }

    // Decoration deletion.
    if (IsKeyPressed(KEY_D)) {
      erase_if(map.decorations, [&](const auto& decoration) {
        return CheckCollisionPointRec(currentMousePos, decoration.frame());
      });
    }

    // Trampoline creation.
    if (IsKeyDown(KEY_T) && IsMouseButtonPressed(2)) {
      map.trampolines.emplace_back(dx(GetMousePosition(), offset));
    }

    // Trampoline deletion.
    if (IsKeyDown(KEY_D)) {
      erase_if(map.trampolines, [&](const auto& trampoline) {
        return CheckCollisionPointRec(currentMousePos, trampoline.frame());
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

    // Recorded obstacles.
    for (auto& obstacle : map.obstacles) {
      Color obstacleColor{BROWN};
      Rectangle obstacleAbsFrame = dx(obstacle.frame(), -offset);
      if (CheckCollisionPointRec(currentMousePos, obstacleAbsFrame))
        obstacleColor = RED;
      DrawRectangleRec(obstacleAbsFrame, obstacleColor);
    }

    // Recorded decorations.
    for (auto& decoration : map.decorations) {
      Color decorationColor{ORANGE};
      Rectangle decorationAbsFrame = dx(decoration.frame(), -offset);
      if (CheckCollisionPointRec(currentMousePos, decorationAbsFrame))
        decorationColor = RED;
      DrawRectangleRec(decorationAbsFrame, decorationColor);
    }

    // Recorded trampolines.
    for (auto& trampoline : map.trampolines) {
      Color trampolineColor{ORANGE};
      Rectangle trampolineAbsFrame = dx(trampoline.frame(), -offset);
      if (CheckCollisionPointRec(currentMousePos, trampolineAbsFrame))
        trampolineColor = RED;
      DrawRectangleRec(trampolineAbsFrame, trampolineColor);
    }

    // Current point cross.
    if (selectedDecoration < 0 && selectedObstacle < 0 && !IsKeyDown(KEY_T)) {
      DrawLineEx(Vector2{(float)currentGridX - offset,
                         (float)(currentGridY - GRID_SIZE)},
                 Vector2{(float)currentGridX - offset,
                         float(currentGridY + GRID_SIZE)},
                 3, DARKBROWN);
      DrawLineEx(Vector2{(float)(currentGridX - GRID_SIZE - offset),
                         (float)(currentGridY)},
                 Vector2{(float)(currentGridX + GRID_SIZE - offset),
                         (float)(currentGridY)},
                 3, DARKBROWN);
    }

    // Current line.
    if (lineStart.has_value()) {
      DrawLineEx(dx(lineStart.value().v2(), -offset),
                 dx(currentCoord.v2(), -offset), 3, DARKBLUE);
    }

    // Current obstacle.
    if (selectedObstacle >= 0) {
      DrawRectangleRec(
          Rectangle{(float)currentX - offset, (float)currentY,
                    (float)obstacleFramePreset[selectedObstacle].x,
                    (float)obstacleFramePreset[selectedObstacle].y},
          MAGENTA);
    }

    // Current decoration.
    if (selectedDecoration >= 0) {
      DrawRectangleRec(
          Rectangle{(float)currentX - offset, (float)currentY,
                    (float)decorationFramePreset[selectedDecoration].x,
                    (float)decorationFramePreset[selectedDecoration].y},
          MAGENTA);
    }

    // Current trampoline.
    if (IsKeyDown(KEY_T)) {
      DrawRectangleRec(
          Rectangle{(float)currentX - offset, (float)currentY,
                    (float)TRAMPOLINE_WIDTH, (float)TRAMPOLINE_HEIGHT},
          DARKPURPLE);
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
