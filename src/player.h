#pragma once

#include <memory>
#include <vector>

#include "assets.h"
#include "defs.h"
#include "map.h"
#include "particles.h"
#include "physics.h"
#include "raylib.h"
#include "sprite.h"
#include "util.h"

using namespace std;

struct Player : Physics::Object {
  vector<unique_ptr<Physics::Behaviour>> behaviours;
  Sprite runSprite{{"run_0", "run_1", "run_2", "run_3", "run_4", "run_5"}, 3};
  Sprite jumpSprite{{"jump_0", "jump_1", "jump_2", "jump_3"}, 2};
  Sprite deadSprite{{"dead_0", "dead_1"}, 8};
  Interpolator deadRotation{0.0f, 360.0f, 20.0f};

  vector<Sprinkler> blood{};

  Player(shared_ptr<Map>* map) : Physics::Object(map) {
    behaviours.push_back(make_unique<Physics::Jumping>());
    behaviours.push_back(make_unique<Physics::ForwardMoving>());
    behaviours.push_back(make_unique<Physics::Gravity>());
    behaviours.push_back(make_unique<Physics::GroundAwareness>());

    reset();
  }

  void update() {
    for (auto& behaviour : behaviours) {
      behaviour->update(this);
    }

    runSprite.update();
    jumpSprite.update();
    deadSprite.update();

    if (!isDead() && pos.y > GetScreenHeight()) {
      LOG("Played died: under screen");
      debug();
      kill();
    }

    if (!isDead() && unsavableBelowGround()) {
      LOG("Player died: below ground");
      debug();
      kill();
    }

    if (isDead()) {
      deadRotation.update();

      blood.emplace_back(pos, map);
      for (auto& bloodCell : blood) bloodCell.update();
    }
  }

  void draw(int xOffset) const {
    Vector2 framePos;

    if (isDead()) {
      Vector2 framePos =
          dxy(pos, -xOffset - (PLAYER_DEAD_WIDTH >> 1), -PLAYER_DEAD_HEIGHT);
      deadSprite.drawRotated(framePos, deadRotation.v);

      for (const auto& bloodCell : blood) bloodCell.draw(xOffset);
    } else if (inAir()) {
      framePos =
          dxy(pos, -xOffset - (PLAYER_JUMP_WIDTH >> 1), -PLAYER_JUMP_HEIGHT);
      jumpSprite.draw(framePos);
    } else {
      framePos = dxy(pos, -xOffset - (PLAYER_WIDTH >> 1), -PLAYER_HEIGHT);
      runSprite.draw(framePos);
    }
  }

  inline int width() const { return PLAYER_WIDTH; }
  inline int height() const { return PLAYER_HEIGHT; }

  void reset() {
    pos.x = 0.0f;
    pos.y = 0.0f;
    v.y = 0.0f;
    v.x = PLAYER_HORIZONTAL_SPEED;
    dead = false;
    deadRotation.reset();
    blood.clear();
  }

  Rectangle frame() const {
    return Rectangle{pos.x - (width() >> 1), pos.y - height(), (float)width(),
                     (float)height()};
  }

  void kill() {
    if (isDead()) return;

    Physics::Object::kill();

    // One last bump animation.
    v.y = PLAYER_JUMP_SMALL_V;
    pos.y += PLAYER_JUMP_SMALL_V;
  }
};
