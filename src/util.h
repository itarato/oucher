#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include "raylib.h"

using namespace std;

inline Vector2 dx(Vector2 v, float offset) {
  return Vector2{v.x + offset, v.y};
}

inline Rectangle dx(Rectangle r, float offset) {
  return Rectangle{r.x + offset, r.y, r.width, r.height};
}

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

struct Map {
  int w{};
  vector<Line> lines{};

  Map(int w, vector<Line> lines) : w(w), lines(lines) {}

  Map(vector<Vector2> anchors) {
    Vector2 current = anchors[0];

    for (int i = 1; i < (int)anchors.size(); i++) {
      Vector2 next = anchors[i];

      lines.emplace_back(current, next);

      current = next;
      w = current.x;
    }
  }
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
