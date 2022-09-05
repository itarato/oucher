#pragma once

#include "defs.h"
#include "raylib.h"

using namespace std;

struct Trampoline {
  Vector2 pos{};

  Trampoline(Vector2 pos) : pos(pos) {}

  void update() {}

  void draw() const { DrawRectangleRec(frame(), VIOLET); }

  Rectangle frame() const {
    return Rectangle{pos.x, pos.y, TRAMPOLINE_WIDTH, TRAMPOLINE_HEIGHT};
  }
};
