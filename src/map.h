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

struct Map {
  int w{};
  vector<Line> lines{};

  vector<Obstacle> obstacles{};
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

    int obstacleLen;
    file >> obstacleLen;

    for (int j = 0; j < obstacleLen; j++) {
      int oType, oX, oY;
      file >> oType >> oX >> oY;

      obstacles.emplace_back((ObstacleType)oType,
                             Vector2{(float)oX, (float)oY});
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
    for (auto& line : lines) line.draw_line_only(xOffset);
    for (auto& obstacle : obstacles) obstacle.draw(xOffset);
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

      LOG("Surface cache: %d -> %d", i, (int)y);
    }

    lastCachedSurfaceY = untilIdx;
  }
};
