#pragma once

#include <cmath>

#include "defs.h"
#include "raylib.h"

using namespace std;

namespace Physics {
struct Object {
  Vector2 pos{};
  Vector2 v{};
  float distanceFromGround{};
  bool dead{false};

  void update() {
    pos.y += v.y;
    pos.x += v.x;
    v.y = min(v.y, PLAYER_GRAVITY_MAX_ACCELERATE);
  }

  inline bool onGround() const {
    return fabs(distanceFromGround) < PLAYER_ON_GROUND_TRESHOLD;
  }

  inline void kill() { dead = true; }

  bool isDead() const { return dead; }
};

struct Behaviour {
  virtual void update(Object* object) = 0;
};

struct Moving : Behaviour {
  void update(Object* object) {
    if (!object->isDead()) {
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
    } else {
      object->v.x *= 0.95;
    }
  }
};

struct Friction : Behaviour {
  const float slowDown{0.95f};

  void update(Object* object) {
    if (fabs(object->v.x) < FRICTION_ZERO_TRESHOLD) {
      object->v.x = 0.0f;
    } else {
      object->v.x *= slowDown;
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

struct GroundAwareness : Behaviour {
  shared_ptr<Map>* map{nullptr};

  GroundAwareness(shared_ptr<Map>* map) : Behaviour(), map(map) {}

  void update(Object* object) {
    float surfaceY = (*map)->surfaceYAtX(object->pos.x);
    if (surfaceY < object->pos.y) {
      object->v.y = 0.0f;

      if (surfaceY > (object->pos.y - GROUND_AWARENESS_LIFT_THRESHOLD)) {
        object->pos.y = surfaceY;
      }
    }
  }
};
};  // namespace Physics
