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
