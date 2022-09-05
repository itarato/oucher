#pragma once

#include "defs.h"
#include "raylib.h"
#include "util.h"

using namespace std;

struct Trampoline {
  Vector2 pos{};

  Trampoline(Vector2 pos) : pos(pos) {}

  void update() {}

  void draw(int xOffset) const {
    DrawRectangleRec(dx(frame(), -xOffset), VIOLET);
  }

  Rectangle frame() const {
    return Rectangle{pos.x, pos.y, TRAMPOLINE_WIDTH, TRAMPOLINE_HEIGHT};
  }
};
