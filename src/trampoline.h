#pragma once

#include "assets.h"
#include "defs.h"
#include "raylib.h"
#include "util.h"

using namespace std;

struct Trampoline {
  Vector2 pos{};

  Trampoline(Vector2 pos) : pos(pos) {}

  void update() {}

  void draw(int xOffset) const {
    DrawTextureV(*assets.texture("trampoline"), Vector2{pos.x - xOffset, pos.y},
                 WHITE);
  }

  Rectangle frame() const {
    return Rectangle{pos.x, pos.y, TRAMPOLINE_WIDTH, TRAMPOLINE_HEIGHT};
  }

  Rectangle activationFrame() const {
    return Rectangle{
        pos.x - TRAMPOLINE_DETECTION_HPADDING,
        pos.y - TRAMPOLINE_DETECTION_HEIGHT - TRAMPOLINE_DETECTION_PADDING,
        TRAMPOLINE_WIDTH + (TRAMPOLINE_DETECTION_HPADDING << 1),
        TRAMPOLINE_DETECTION_HEIGHT};
  }
};
