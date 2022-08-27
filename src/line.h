#pragma once

#include <algorithm>
#include <vector>

#include "raylib.h"
#include "util.h"

using namespace std;

struct Line {
  Vector2 a{};
  Vector2 b{};

  inline float minX() const { return min(a.x, b.x); }
  inline float maxX() const { return max(a.x, b.x); }
  inline float minY() const { return min(a.y, b.y); }
  inline float maxY() const { return max(a.y, b.y); }

  void draw(int xOffset) {
    if (maxX() < xOffset) return;
    if (minX() - xOffset > GetScreenWidth()) return;

    DrawRectangle(minX() - xOffset, maxY(), maxX() - minX(),
                  GetScreenHeight() - maxY(), LIGHTGRAY);

    Vector2 c;
    if (a.y < b.y) {
      c = Vector2{a.x, b.y};
    } else {
      c = Vector2{b.x, a.y};
    }

    DrawTriangle(dx(a, -xOffset), dx(c, -xOffset), dx(b, -xOffset), LIGHTGRAY);
  }
};

float deltaYPointToLineList(Vector2 p, const vector<Line>& lines) {
  float dy = INFINITY;

  for (auto& line : lines) {
    float currentDY = deltaYPointToLine(p, line.a, line.b);
    if (dy > currentDY) dy = currentDY;
  }

  return dy;
}
