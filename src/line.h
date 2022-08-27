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

  float delta(Vector2 p) const {
    Vector2 left, right;

    if (a.x < b.x) {
      left = a;
      right = b;
    } else if (b.x < a.x) {
      left = b;
      right = a;
    } else {
      if (p.x == a.x) {
        return min(a.y, b.y) - p.y;
      } else {
        return INFINITY;
      }
    }

    if (p.x < left.x || p.x > right.x) return INFINITY;

    float slope = (right.y - left.y) / (right.x - left.x);
    float offsX = left.x;
    float offsY = left.y;
    float lineY = (slope * (p.x - offsX)) + offsY;

    return lineY - p.y;
  }
};

float deltaYPointToLineList(Vector2 p, const vector<Line>& lines) {
  float dy = INFINITY;

  for (auto& line : lines) {
    float currentDY = line.delta(p);
    if (dy > currentDY) dy = currentDY;
  }

  return dy;
}
