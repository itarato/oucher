#pragma once

#include <cmath>

#include "defs.h"
#include "raylib.h"

using namespace std;

namespace Physics {
struct Object {
  Vector2 pos{};
  Vector2 v{};
  shared_ptr<Map>* map{nullptr};
  bool dead{false};

  Object(shared_ptr<Map>* map) : map(map) {}

  bool onGround() const {
    float surfaceY = (*map)->surfaceYAtX(pos.x);

    return between(pos.y, surfaceY - PLAYER_ON_GROUND_TRESHOLD_ABOVE,
                   surfaceY + PLAYER_LIFT_FROM_BELOW_TRESHOLD);
  }

  bool inAir() const {
    float surfaceY = (*map)->surfaceYAtX(pos.x);
    return pos.y < surfaceY - PLAYER_ON_GROUND_TRESHOLD_ABOVE;
  }

  bool unsavableBelowGround() const {
    float surfaceY = (*map)->surfaceYAtX(pos.x);

    return pos.y > surfaceY + PLAYER_LIFT_FROM_BELOW_TRESHOLD;
  }

  virtual void kill() { dead = true; }

  bool isDead() const { return dead; }

  bool isGoingUpwards() const { return v.y < 0.0; }
  bool isGoingDownwards() const { return v.y > 0.0; }

  void debug() const {
    LOG("Physics Object:\n\tpos x: %.2f y: %.2f\n\tv x: %.2f y: "
        "%.2f\n\tsurface y: "
        "%.2f",
        pos.x, pos.y, v.x, v.y, (*map)->surfaceYAtX(pos.x));
  }
};

struct Behaviour {
  virtual void update(Object* object) = 0;
};

struct Jumping : Behaviour {
  void update(Object* object) {
    if (object->isDead()) return;

    if (IsKeyPressed(KEY_UP) && object->onGround()) {
      object->v.y = PLAYER_JUMP_V;
    }

    if (IsKeyPressed(KEY_LEFT) && object->onGround()) {
      object->v.y = PLAYER_JUMP_SMALL_V;
      object->v.x = PLAYER_HORIZONTAL_SPEED_SLOW;
    }

    if (IsKeyPressed(KEY_RIGHT) && object->onGround()) {
      object->v.y = PLAYER_JUMP_SMALL_V;
      object->v.x = PLAYER_HORIZONTAL_SPEED_FAST;
    }
  }
};

struct ForwardMoving : Behaviour {
  void update(Object* object) {
    if (!object->isDead()) {
      if (object->v.x > PLAYER_HORIZONTAL_SPEED + 1.0f) {
        object->v.x = PLAYER_HORIZONTAL_SPEED +
                      (object->v.x - PLAYER_HORIZONTAL_SPEED) * 0.9f;
      } else if (object->v.x < PLAYER_HORIZONTAL_SPEED - 1.0f) {
        object->v.x = PLAYER_HORIZONTAL_SPEED -
                      (PLAYER_HORIZONTAL_SPEED - object->v.x) * 0.98f;
      } else {
        object->v.x = PLAYER_HORIZONTAL_SPEED;
      }
    } else {
      if (object->onGround()) object->v.x *= 0.95;
    }

    object->pos.y += object->v.y;
    object->pos.x += object->v.x;
  }
};

struct BasicMoving : Behaviour {
  void update(Object* object) {
    object->pos.y += object->v.y;
    object->pos.x += object->v.x;
  }
};

struct Friction : Behaviour {
  const float slowDown{0.98f};

  void update(Object* object) {
    if (fabs(object->v.x) < FRICTION_ZERO_TRESHOLD) {
      object->v.x = 0.0f;
    } else {
      object->v.x *= slowDown;
    }
  }
};

struct Gravity : Behaviour {
  const float accelerate = 1.1f;
  const float decelerate = 0.87f;
  const float max_accelerate = 8.0f;
  const float fallback_threshold = 1.0f;

  void update(Object* object) {
    if (between(object->v.y, -fallback_threshold,
                0.0)) {  // Falling back from jump.
      object->v.y = fallback_threshold;
    } else if (object->v.y < 0.0f) {
      object->v.y *= decelerate;
    } else {
      object->v.y = min(object->v.y * accelerate, max_accelerate);
    }
  }
};

struct GroundAwareness : Behaviour {
  void update(Object* object) {
    if (!object->onGround()) return;
    // Object is on or below ground.

    object->v.y = 0.0f;

    float surfaceY = (*object->map)->surfaceYAtX(object->pos.x);
    object->pos.y = surfaceY;
  }
};
};  // namespace Physics
