#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include "raylib.h"

using namespace std;

inline Vector2 dx(Vector2 v, float offset) {
  return Vector2{v.x + offset, v.y};
}

struct Line {
  Vector2 a{};
  Vector2 b{};

  inline float minX() const { return min(a.x, b.x); }
  inline float maxX() const { return max(a.x, b.x); }
  inline float minY() const { return min(a.y, b.y); }
  inline float maxY() const { return max(a.y, b.y); }

  void draw(int xOffset) {
    // TODO: filter out off-screen lines.
    DrawRectangle(minX() - xOffset, maxY(), maxX() - xOffset, GetScreenHeight() - maxY(), LIGHTGRAY);

    Vector2 c;
    if (a.y < b.y) {
      c = Vector2{a.x, b.y};
    } else {
      c = Vector2{b.x, a.y};
    }

    DrawTriangle(dx(a, -xOffset), c, b, LIGHTGRAY);
  }
};

struct Map {
  int w{};
  int h{};

  vector<Line> lines{};

  Map(int w, int h, vector<Line> lines)
      : w(w), h(h), lines(lines) {}
};

float deltaYPointToLine(Vector2 p, Vector2 l1, Vector2 l2) {
  Vector2 left, right;

  if (l1.x < l2.x) {
    left = l1;
    right = l2;
  } else if (l2.x < l1.x) {
    left = l2;
    right = l1;
  } else {
    if (p.x == l1.x) {
      return min(l1.y, l2.y) - p.y;
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

float deltaYPointToLineList(Vector2 p, vector<Line>& lines) {
  float dy = INFINITY;

  for (auto& line : lines) {
    float currentDY = deltaYPointToLine(p, line.a, line.b);
    if (dy > currentDY) dy = currentDY;
  }

  return dy;
}
