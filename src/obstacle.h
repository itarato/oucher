#pragma once

#include <vector>

#include "raylib.h"
#include "util.h"

using namespace std;

static vector<IntVector2> obstacleFramePreset{
    {20, 30},
    {20, 50},
    {20, 80},
};

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
