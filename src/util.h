#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include "raylib.h"

using namespace std;

struct Point {
  float x{};
  float y{};

  Vector2 v2() const { return Vector2{x, y}; }

  static Point fromMouse() {
    return Point{(float)GetMouseX(), (float)GetMouseY()};
  }
};

struct Triangle {
  Point a{};
  Point b{};
  Point c{};
};

struct Line {
  Point a{};
  Point b{};

  inline float minX() const { return min(a.x, b.x); }
  inline float maxX() const { return max(a.x, b.x); }
  inline float minY() const { return min(a.y, b.y); }
  inline float maxY() const { return max(a.y, b.y); }
};

struct Map {
  int w{};
  int h{};

  vector<Triangle> triangles{};

  Map(int w, int h, vector<Triangle> triangles)
      : w(w), h(h), triangles(triangles) {}
};

namespace Util {

float deltaYPointToLine(Point p, Point l1, Point l2) {
  Point left, right;

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

float deltaYPointToTriangle(Point p, Triangle& triangle) {
  float dy = INFINITY;

  auto dy1 = Util::deltaYPointToLine(p, triangle.a, triangle.b);
  auto dy2 = Util::deltaYPointToLine(p, triangle.b, triangle.c);
  auto dy3 = Util::deltaYPointToLine(p, triangle.a, triangle.c);

  if (dy1 < dy) dy = dy1;
  if (dy2 < dy) dy = dy2;
  if (dy3 < dy) dy = dy3;

  return dy;
}

float deltaYPointToTriangleList(Point p, vector<Triangle>& triangles) {
  float dy = INFINITY;

  for (auto& triangle : triangles) {
    float currentDY = deltaYPointToTriangle(p, triangle);
    if (dy > currentDY) dy = currentDY;
  }

  return dy;
}

};  // namespace Util
