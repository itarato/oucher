#pragma once

#include <memory>
#include <vector>

#include "map.h"
#include "physics.h"
#include "raylib.h"
#include "util.h"

using namespace std;

struct Sprinkler : Physics::Object {
  const Color color = RED;
  vector<unique_ptr<Physics::Behaviour>> behaviours;

  int radius{};

  Sprinkler(Vector2 thePos, shared_ptr<Map>* map) : Physics::Object(map) {
    pos = thePos;

    radius = rand_range(1, 3);
    pos.x += rand_range(-5, 5);
    pos.y += rand_range(-2, 3);
    v.x = randf(-5.0f, 5.0f);
    v.y = randf(-8.0f, 0.0f);

    behaviours.emplace_back(make_unique<Physics::Gravity>());
    behaviours.emplace_back(make_unique<Physics::BasicMoving>());
    behaviours.emplace_back(make_unique<Physics::Friction>());
    behaviours.emplace_back(make_unique<Physics::GroundAwareness>());
  }

  void update() {
    if (pos.y > GetScreenHeight()) return;
    for (auto& behaviour : behaviours) behaviour->update(this);
  }

  void draw(int xOffset) const {
    DrawRectangleV(dx(pos, -xOffset), Vector2{(float)radius, (float)radius},
                   color);
  }
};
