#pragma once

#include <memory>
#include <vector>

#include "assets.h"
#include "defs.h"
#include "physics.h"
#include "raylib.h"
#include "sprite.h"
#include "util.h"

using namespace std;

struct Player : Physics::Object {
  vector<unique_ptr<Physics::Behaviour>> behaviours;
  Sprite sprite{{"run_0", "run_1", "run_2", "run_3", "run_4", "run_5"}, 2};
  Interpolator deadRotation{0.0f, 360.0f, 20.0f};

  Player() {
    behaviours.push_back(make_unique<Physics::Moving>());
    behaviours.push_back(make_unique<Physics::Gravity>());

    reset();
  }

  void update(float newDistanceFromGround) {
    distanceFromGround = newDistanceFromGround;

    if (distanceFromGround != INFINITY) {
      if (distanceFromGround < 0.0 &&
          distanceFromGround > PLAYER_LIFT_FROM_BELOW_TRESHOLD) {
        pos.y += distanceFromGround;
        v.y = 0.0f;
      }
    }

    for (auto& behaviour : behaviours) {
      behaviour->update(this);
    }
    Physics::Object::update();

    sprite.update();

    if (pos.y > GetScreenHeight()) kill();
    if (distanceFromGround < PLAYER_LIFT_FROM_BELOW_TRESHOLD) kill();

    if (isDead()) {
      deadRotation.update();
    }
  }

  void draw(int xOffset) const {
    Vector2 framePos = dxy(pos, -xOffset - (PLAYER_WIDTH >> 1), -PLAYER_HEIGHT);

    if (isDead()) {
      Texture2D deadTexture = *assets.texture("dead");
      DrawTexturePro(
          deadTexture,
          Rectangle{0.0f, 0.0f, (float)deadTexture.width,
                    (float)deadTexture.height},
          Rectangle{pos.x - xOffset - (deadTexture.width >> 1),
                    pos.y - (deadTexture.height >> 1), (float)deadTexture.width,
                    (float)deadTexture.height},
          Vector2{(float)(deadTexture.width >> 1),
                  (float)(deadTexture.height >> 1)},
          deadRotation.v, WHITE);
    } else if (distanceFromGround > PLAYER_ON_GROUND_TRESHOLD) {
      DrawTextureV(*assets.texture("jump"), framePos, WHITE);
    } else {
      sprite.draw(framePos);
    }
  }

  inline int width() const { return 20; }
  inline int height() const { return 30; }

  void reset() {
    pos.x = 0.0f;
    pos.y = 0.0f;
    v.y = 0.0f;
    v.x = PLAYER_HORIZONTAL_SPEED;
    dead = false;
    deadRotation.reset();
  }

  Rectangle frame() const {
    return Rectangle{pos.x - (width() >> 1), pos.y - height(), (float)width(),
                     (float)height()};
  }
};
