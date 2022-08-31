#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include "raylib.h"

using namespace std;

struct IntVector2 {
  int x{};
  int y{};

  Vector2 v2() const { return Vector2{(float)x, (float)y}; }
};

inline Vector2 dx(Vector2 v, float offset) {
  return Vector2{v.x + offset, v.y};
}

inline Vector2 dxy(Vector2 v, float xOffset, float yOffset) {
  return Vector2{v.x + xOffset, v.y + yOffset};
}

inline Rectangle dx(Rectangle r, float offset) {
  return Rectangle{r.x + offset, r.y, r.width, r.height};
}

struct Countdown {
  const int n;
  int counter{};
  bool enabled{false};

  Countdown(int n) : n(n) {}

  void update() {
    if (!enabled) return;

    if (counter >= 0) counter--;
  }

  bool isComplete() const { return counter < 0; }

  void reset() {
    counter = n;
    enabled = false;
  }

  void restart() {
    reset();
    enabled = true;
  }
};
