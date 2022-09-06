#pragma once

#include <vector>

#include "assets.h"
#include "raylib.h"
#include "util.h"

using namespace std;

static vector<IntVector2> obstacleFramePreset{
    {28, 28},
    {28, 56},
    {28, 84},
    {33, 11},
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
    const char* name = TextFormat("obstacle_%d", (int)type);
    DrawTextureV(*assets.texture(name), Vector2{pos.x - xOffset, pos.y}, WHITE);
  }
};
