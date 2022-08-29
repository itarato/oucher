#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "debug.h"
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

  Map() = default;

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

  Map(const char* fileName) {
    ifstream file{fileName};

    if (!file.is_open()) {
      ERROR("Map file %s cannot be opened", fileName);
      return;
    }

    int coordLen;
    file >> coordLen;

    for (int i = 0; i < coordLen; i++) {
      int x1, y1, x2, y2;

      file >> x1 >> y1 >> x2 >> y2;

      lines.emplace_back(IntVector2{x1, y1}, IntVector2{x2, y2});

      if (x1 > w) w = x1;
      if (x2 > w) w = x2;
    }

    file.close();
  }

  inline float deltaYPointToSurface(Vector2 p) const {
    return deltaYPointToLineList(p, lines);
  }

  void draw(int xOffset) {
    for (auto& line : lines) line.draw(xOffset);
    for (auto& obstacle : obstacles) obstacle.draw(xOffset);
  }
};
