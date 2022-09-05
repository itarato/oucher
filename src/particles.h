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

  shared_ptr<Map>* map{nullptr};

  Sprinkler(Vector2 thePos, shared_ptr<Map>* map) : map(map) {
    pos = thePos;

    radius = rand_range(1, 2);
    pos.x += rand_range(-15, 15);
    pos.y += rand_range(-2, 3);
    v.x = randf(-5.0f, 5.0f);
    v.y = randf(-8.0f, 0.0f);

    behaviours.emplace_back(make_unique<Physics::Gravity>());
    behaviours.emplace_back(make_unique<Physics::Friction>());
    behaviours.emplace_back(make_unique<Physics::GroundAwareness>(map));
  }

  void update() {
    if (pos.y > GetScreenHeight()) return;

    for (auto& behaviour : behaviours) behaviour->update(this);
    Physics::Object::update();
  }

  void draw() const { DrawCircleV(pos, (float)radius, color); }
};
