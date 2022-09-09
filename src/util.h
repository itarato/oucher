#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
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

inline constexpr bool between(float v, float lhs, float rhs) {
  return lhs <= v && v <= rhs;
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

struct Interpolator {
  float v{};
  const float start{};
  const float target{};
  const float step{};
  bool completed{false};

  Interpolator(float start, float target, float step)
      : v(start), start(start), target(target), step(step) {}

  void update() {
    if (completed) return;

    bool beforeCmp = v <= target;
    v += step;
    bool afterCmp = v <= target;

    if (beforeCmp ^ afterCmp) {
      completed = true;
      v = target;
    }
  }

  void reset() {
    v = start;
    completed = false;
  }
};

int rand_range(int min, int max) { return (rand() % (max - min + 1)) + min; }

float randf() { return (float)(rand() & 0xFFFF) / (float)0xFFFF; }

float randf(float min, float max) { return randf() * (max - min) + min; }
