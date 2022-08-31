#pragma once

#include <vector>

#include "assets.h"
#include "raylib.h"

using namespace std;

struct Sprite {
  vector<const char*> frameNames{};
  int frameSpan{};
  int frame{0};

  int counter{0};

  Sprite(vector<const char*> frameNames, int frameSpan)
      : frameNames(frameNames), frameSpan(frameSpan) {}

  void update() {
    counter = (counter + 1) % frameSpan;
    if (counter == 0) frame = (frame + 1) % frameNames.size();
  }

  void draw(Vector2 pos) const {
    const Texture2D* texture = assets.texture(frameNames[frame]);
    DrawTextureV(*texture, pos, WHITE);
  }
};
