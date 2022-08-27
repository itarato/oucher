#pragma once

#include "raylib.h"
#include "util.h"

using namespace std;

struct Obstacle {
  Rectangle frame;

  Obstacle(Rectangle frame) : frame(frame) {}

  void draw(int xOffset) { DrawRectangleRec(dx(frame, -xOffset), BROWN); }
};
