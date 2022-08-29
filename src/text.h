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
  int backgroundPadding{12};
  optional<Color> backgroundColor{nullopt};

  Text(const char* s) : s(s) {}
  Text(const Text& t) = delete;
  Text(Text&& t) = default;

  static Text build(const char* s) { return Text(s); }

  Text& toCenter() {
    hAlign = HorizontalAlign::Center;
    vAlign = VerticalAlign::Center;

    return *this;
  }

  Text& toBottomLeft() {
    hAlign = HorizontalAlign::Left;
    vAlign = VerticalAlign::Bottom;

    return *this;
  }

  Text& withColor(Color c) {
    color = c;
    return *this;
  }

  Text& withBackgroundColor(Color c) {
    backgroundColor = c;
    return *this;
  }

  Text& toLarge() {
    fontSize = 20;
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

    if (backgroundColor.has_value()) {
      DrawRectangle(x - backgroundPadding, y - backgroundPadding,
                    width + (backgroundPadding << 1),
                    fontSize + (backgroundPadding << 1),
                    backgroundColor.value());
    }

    DrawText(s, x, y, fontSize, color);
  }
};
