#pragma once

#include <memory>
#include <vector>

#include "assets.h"
#include "defs.h"
#include "raylib.h"
#include "sprite.h"
#include "util.h"

using namespace std;

#define PLAYER_WIDTH 20
#define PLAYER_HEIGHT 30
#define PLAYER_JUMP_V -20.0f
#define PLAYER_JUMP_SMALL_V -8.0f
#define PLAYER_HORIZONTAL_SPEED 2.0f
#define PLAYER_HORIZONTAL_SPEED_FAST 12.0f
#define PLAYER_GRAVITY_SLOWDOWN 0.87f
#define PLAYER_GRAVITY_ACCELERATE 1.1f
#define PLAYER_GRAVITY_BACKFALL_TRESHOLD 1.0f
#define PLAYER_GRAVITY_MAX_ACCELERATE 8.0f
#define PLAYER_LIFT_FROM_BELOW_TRESHOLD -15.0f
#define PLAYER_ON_GROUND_TRESHOLD 5.0f

namespace Physics {
struct Object {
  Vector2 pos{};
  Vector2 v{};
  float distanceFromGround{};

  void update() {
    pos.y += v.y;
    pos.x += v.x;
    v.y = min(v.y, PLAYER_GRAVITY_MAX_ACCELERATE);
  }

  inline bool onGround() const {
    return fabs(distanceFromGround) < PLAYER_ON_GROUND_TRESHOLD;
  }
};

struct Behaviour {
  virtual void update(Object* object) = 0;
};

struct Moving : Behaviour {
  void update(Object* object) {
    if (IsKeyPressed(KEY_UP) && object->onGround()) {
      object->v.y = PLAYER_JUMP_V;
    }
    if (IsKeyPressed(KEY_LEFT) && object->onGround()) {
      object->v.y = PLAYER_JUMP_SMALL_V;
    }
    if (IsKeyPressed(KEY_RIGHT) && object->onGround()) {
      object->v.y = PLAYER_JUMP_SMALL_V;
      object->v.x = PLAYER_HORIZONTAL_SPEED_FAST;
    }

    if (object->v.x > PLAYER_HORIZONTAL_SPEED + 1.0f) {
      object->v.x = PLAYER_HORIZONTAL_SPEED +
                    (object->v.x - PLAYER_HORIZONTAL_SPEED) * 0.9;
    } else {
      object->v.x = PLAYER_HORIZONTAL_SPEED;
    }
  }
};

struct Gravity : Behaviour {
  void update(Object* object) {
    if (fabs(object->v.y) < PLAYER_GRAVITY_BACKFALL_TRESHOLD) {
      object->v.y = PLAYER_GRAVITY_BACKFALL_TRESHOLD;
    } else if (object->v.y < 0.0f) {
      object->v.y *= PLAYER_GRAVITY_SLOWDOWN;
    } else {
      object->v.y *= PLAYER_GRAVITY_ACCELERATE;
    }
  }
};
};  // namespace Physics

struct Player : Physics::Object {
  vector<unique_ptr<Physics::Behaviour>> behaviours;
  Sprite sprite{{"run_0", "run_1", "run_2", "run_3", "run_4", "run_5"}, 2};

  Player() {
    behaviours.push_back(make_unique<Physics::Moving>());
    behaviours.push_back(make_unique<Physics::Gravity>());

    init();
  }

  void init() {
    v.y = 0.0f;
    v.x = PLAYER_HORIZONTAL_SPEED;
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
  }

  void draw(int xOffset) const {
    Vector2 framePos = dxy(pos, -xOffset - (PLAYER_WIDTH >> 1), -PLAYER_HEIGHT);

    if (distanceFromGround > PLAYER_ON_GROUND_TRESHOLD) {
      DrawTextureV(*assets.texture("jump"), framePos, WHITE);
    } else {
      sprite.draw(framePos);
    }
  }

  inline int width() const { return 20; }
  inline int height() const { return 30; }

  void reset() {
    pos.x = 0;
    pos.y = 0;
    v.y = 0;
    v.x = PLAYER_HORIZONTAL_SPEED;
  }

  Rectangle frame() const {
    return Rectangle{pos.x - (width() >> 1), pos.y - height(), (float)width(),
                     (float)height()};
  }

  bool isDead() const {
    if (pos.y > GetScreenHeight()) return true;
    if (distanceFromGround < PLAYER_LIFT_FROM_BELOW_TRESHOLD) return true;

    return false;
  }
};
