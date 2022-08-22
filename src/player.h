#pragma once

#include <memory>
#include <vector>

#include "raylib.h"
#include "util.h"

using namespace std;

#define PLAYER_JUMP_V -20.0f
#define PLAYER_HORIZONTAL_SPEED 5.0f
#define PLAYER_GRAVITY_SLOWDOWN 0.87f
#define PLAYER_GRAVITY_ACCELERATE 1.1f
#define PLAYER_GRAVITY_BACKFALL_TRESHOLD 1.0f
#define PLAYER_GRAVITY_MAX_ACCELERATE 8.0f
#define PLAYER_LIFT_FROM_BELOW_TRESHOLD -10.0f

namespace Physics {
struct Object {
  Point pos{};
  Point v{};

  void update() {
    pos.y += v.y;
    pos.x += v.x;
    v.y = min(v.y, PLAYER_GRAVITY_MAX_ACCELERATE);
  }
};

struct Behaviour {
  virtual void update(Object* object) = 0;
};

struct Moving : Behaviour {
  void update(Object* object) {
    if (IsKeyPressed(KEY_SPACE)) {
      object->v.y = PLAYER_JUMP_V;
    }

    if (IsKeyDown(KEY_LEFT)) {
      object->v.x = -PLAYER_HORIZONTAL_SPEED;
    }
    if (IsKeyDown(KEY_RIGHT)) {
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

  Player() {
    behaviours.push_back(make_unique<Physics::Moving>());
    behaviours.push_back(make_unique<Physics::Gravity>());

    init();
  }

  void init() {
    v.y = 0.0f;
    v.x = PLAYER_HORIZONTAL_SPEED;
  }

  void update() {
    for (auto& behaviour : behaviours) {
      behaviour->update(this);
    }
    Physics::Object::update();
  }

  void draw() { DrawCircle(pos.x, pos.y, 6.0f, DARKPURPLE); }
};
