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
  ObstacleType type{ObstacleType::Small};
  Vector2 pos{};

  Obstacle(ObstacleType type, Vector2 pos) : type(type), pos(pos) {}

  Rectangle frame() const {
    return Rectangle{pos.x, pos.y, (float)obstacleFramePreset[(int)type].x,
                     (float)obstacleFramePreset[(int)type].y};
  }
  void draw(int xOffset) const {
    DrawRectangleRec(dx(frame(), -xOffset), BROWN);
  }
};
