#pragma once

#include <vector>

#include "line.h"
#include "obstacle.h"
#include "raylib.h"
#include "util.h"

using namespace std;

struct Map {
  int w{};
  vector<Line> lines{};

  vector<Obstacle> obstacles{
      Obstacle({400, 300, 60, 80}),
  };

  Map(int w, vector<Line> lines) : w(w), lines(lines) {}

  Map(vector<IntVector2> anchors) {
    IntVector2 current = anchors[0];

    for (int i = 1; i < (int)anchors.size(); i++) {
      IntVector2 next = anchors[i];

      lines.emplace_back(current, next);

      current = next;
      w = current.x;
    }
  }

  inline float deltaYPointToSurface(Vector2 p) const {
    return deltaYPointToLineList(p, lines);
  }

  void draw(int xOffset) {
    for (auto& line : lines) line.draw(xOffset);
    for (auto& obstacle : obstacles) obstacle.draw(xOffset);
  }
};
