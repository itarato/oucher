#pragma once

#include "raylib.h"
#include "util.h"

using namespace std;

enum class ObstacleType {
  Small,
  Medium,
  Tall,
};

struct Obstacle {
  Rectangle frame;

  Obstacle(Rectangle frame) : frame(frame) {}

  void draw(int xOffset) const { DrawRectangleRec(dx(frame, -xOffset), BROWN); }
};
