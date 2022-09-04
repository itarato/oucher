#pragma once

#include <memory>
#include <vector>

#include "physics.h"
#include "raylib.h"
#include "util.h"

using namespace std;

struct Sprinkler : Physics::Object {
  const Color color = RED;
  vector<unique_ptr<Physics::Behaviour>> behaviours;

  int radius{};

  Sprinkler(Vector2 thePos) {
    pos = thePos;

    radius = rand_range(1, 4);
    pos.x += rand_range(-3, 3);
    pos.y += rand_range(-2, 3);
    v.x = randf(-5.0f, 5.0f);
    v.y = randf(-4.0f, 0.0f);
  }

  void update() {
    if (pos.y > GetScreenHeight()) return;

    pos.x += v.x;
    pos.y += v.y;
  }

  void draw() const { DrawCircleV(pos, (float)radius, color); }
};
