#pragma once

#include <string>
#include <utility>

#include "map.h"
#include "player.h"
#include "raylib.h"
#include "text.h"
#include "util.h"

#define APP_SCROLL_START_PADDING 200

using namespace std;

vector<Map> default_map_file_list(const char* folder) {
  FilePathList files = LoadDirectoryFiles(folder);

  vector<Map> out{};

  for (int i = 0; i < (int)files.count; i++) {
    out.emplace_back(files.paths[i]);
    LOG("Map file loaded: %s", files.paths[i]);
  }

  return out;
}

enum class AppState {
  Ready,
  Running,
};

struct App {
  int stage{0};

  vector<Map> maps = default_map_file_list("./maps");

  Player player{};

  int offset{0};

  AppState state{AppState::Ready};

  App() = default;

  ~App() {
    ShowCursor();
    CloseWindow();
  }

  void init() {
    InitWindow(1000, 500, "Oucher V0.1 pre-alpha dev build");
    SetTargetFPS(60);
    HideCursor();

    restart();
  }

  void restart() {
    stage = 0;
    player.reset();
    state = AppState::Ready;
  }

  void loop() {
    while (!WindowShouldClose()) {
      update();

      BeginDrawing();
      ClearBackground(RAYWHITE);

      draw();

      EndDrawing();
    }
  }

  void update() {
    if (state == AppState::Running) {
      update_game();
    } else if (state == AppState::Ready) {
      if (IsKeyPressed(KEY_ENTER)) {
        state = AppState::Running;
      }
    }
  }

  void update_game() {
    auto distanceFromGround = maps[stage].deltaYPointToSurface(player.pos);
    player.update(distanceFromGround);

    if (player.pos.x >= maps[stage].w - player.width()) {
      handle_win();
    }
  }

  void draw() {
    if (state == AppState::Running) {
      draw_game();
    } else if (state == AppState::Ready) {
      draw_menu();
    }
  }

  void draw_menu() {
    Text::build(TextFormat("Stage [%d] - Press [Enter]", stage))
        .toCenter()
        .draw();
  }

  void draw_game() {
    maps[stage].draw(xOffset());
    player.draw(xOffset());

    // DrawFPS(4, 4);
  }

  void handle_win() {
    state = AppState::Ready;
    stage = (stage + 1) % maps.size();

    player.reset();
  }

  int xOffset() const {
    if (maps[stage].w < GetScreenWidth()) return 0;
    if (player.pos.x < APP_SCROLL_START_PADDING) return 0;

    return min((int)player.pos.x - APP_SCROLL_START_PADDING,
               maps[stage].w - GetScreenWidth());
  }
};
