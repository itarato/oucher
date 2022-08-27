#pragma once

#include "raylib.h"

using namespace std;

enum class HorizontalAlign {
  Left,
  Center,
  Right,
};

enum class VerticalAlign {
  Top,
  Center,
  Bottom,
};

struct Text {
  const char* s;
  int fontSize{10};
  Color color{DARKGRAY};

  HorizontalAlign hAlign{HorizontalAlign::Left};
  VerticalAlign vAlign{VerticalAlign::Top};

  int padding{12};

  Text(const char* s) : s(s) {}
  Text(const Text& t) = delete;
  Text(Text&& t) = default;

  static Text build(const char* s) { return Text(s); }

  Text& toCenter() {
    hAlign = HorizontalAlign::Center;
    vAlign = VerticalAlign::Center;

    return *this;
  }

  void draw() {
    int width = MeasureText(s, fontSize);
    int x{};
    int y{};

    if (hAlign == HorizontalAlign::Left) {
      x = padding;
    } else if (hAlign == HorizontalAlign::Center) {
      x = (GetScreenWidth() - width - 2 * padding) / 2;
    } else if (hAlign == HorizontalAlign::Right) {
      x = GetScreenWidth() - width - padding;
    }

    if (vAlign == VerticalAlign::Top) {
      y = padding;
    } else if (vAlign == VerticalAlign::Center) {
      y = (GetScreenHeight() - fontSize - 2 * padding) / 2;
    } else if (vAlign == VerticalAlign::Bottom) {
      y = GetScreenHeight() - fontSize - padding;
    }

    DrawText(s, x, y, fontSize, color);
  }
};
