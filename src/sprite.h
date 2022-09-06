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

  void drawRotated(Vector2 pos, float rot) const {
    const Texture2D* texture = assets.texture(frameNames[frame]);

    DrawTexturePro(
        *texture,
        Rectangle{0.0f, 0.0f, (float)texture->width, (float)texture->height},
        Rectangle{pos.x + (texture->width >> 1), pos.y + (texture->height >> 1),
                  (float)texture->width, (float)texture->height},
        Vector2{(float)(texture->width >> 1), (float)(texture->height >> 1)},
        rot, WHITE);
  }
};
