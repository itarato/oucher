#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "debug.h"
#include "line.h"
#include "obstacle.h"
#include "raylib.h"
#include "trampoline.h"
#include "util.h"

using namespace std;

static vector<IntVector2> decorationFramePreset{
    {100, 100},
    {80, 160},
};

enum class DecorationType {
  T1,
  T2,
};

struct Decoration {
  DecorationType type{DecorationType::T1};
  Vector2 pos{};

  Decoration(DecorationType type, Vector2 pos) : type(type), pos(pos) {}

  void draw(int xOffset) const {
    DrawRectangleRec(dx(frame(), -xOffset), ORANGE);
  }

  Rectangle frame() const {
    return Rectangle{pos.x, pos.y, (float)decorationFramePreset[(int)type].x,
                     (float)decorationFramePreset[(int)type].y};
  }
};

struct Map {
  int w{};

  vector<Line> lines{};
  vector<Obstacle> obstacles{};
  vector<Trampoline> trampolines{};
  vector<Decoration> decorations{};

  vector<float> surfaceCache{};

  Map(const char* fileName) {
    ifstream file{fileName};

    if (!file.is_open()) {
      ERROR("Map file %s cannot be opened", fileName);
      return;
    }

    int coordLen;
    file >> coordLen;

    for (int i = 0; i < coordLen; i++) {
      int x1, y1, x2, y2;

      file >> x1 >> y1 >> x2 >> y2;

      lines.emplace_back(IntVector2{x1, y1}, IntVector2{x2, y2});

      if (x1 > w) w = x1;
      if (x2 > w) w = x2;
    }

    int staticObjectLen;
    file >> staticObjectLen;

    for (int j = 0; j < staticObjectLen; j++) {
      int type, x, y;
      file >> type >> x >> y;

      if (type == STATIC_OBJECT_ID_TRAMPOLINE) {
        trampolines.emplace_back(Vector2{(float)x, (float)y});
      } else {
        obstacles.emplace_back((ObstacleType)type, Vector2{(float)x, (float)y});
      }
    }

    file.close();

    LOG("Map loaded: %s", fileName);

    surfaceCache.reserve(w);
    cacheSurfaceYRange(w);
  }

  inline float deltaYPointToSurface(Vector2 p) const {
    return surfaceYAtX((int)p.x) - p.y;
  }

  void draw_ground(int xOffset) const {
    for (auto& line : lines) line.draw(xOffset);
  }

  void draw_not_ground(int xOffset) const {
    for (const auto& line : lines) line.draw_line_only(xOffset);
    for (const auto& obstacle : obstacles) obstacle.draw(xOffset);
    for (const auto& trampoline : trampolines) trampoline.draw(xOffset);
    for (const auto& decoration : decorations) decoration.draw(xOffset);
  }

  bool hasObstacleCollision(Rectangle frame) const {
    for (auto& obstacle : obstacles) {
      if (CheckCollisionRecs(frame, obstacle.frame())) return true;
    }

    return false;
  }

  float surfaceYAtX(int x) const {
    if (x >= w) return INFINITY;

    return surfaceCache[x];
  }

 private:
  int lastCachedSurfaceY{-1};

  void cacheSurfaceYRange(int untilIdx) {
    if (untilIdx > w) untilIdx = w;

    for (int i = lastCachedSurfaceY + 1; i <= untilIdx; i++) {
      float y = INFINITY;

      auto it = find_if(lines.begin(), lines.end(),
                        [&](const auto& line) { return line.matchX(i); });

      if (it != lines.end()) y = it->yAtX((float)i);

      surfaceCache.emplace_back(y);
    }

    lastCachedSurfaceY = untilIdx;
  }
};
