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
