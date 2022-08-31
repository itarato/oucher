#pragma once

#include <string>
#include <utility>

#include "assets.h"
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
  Losing,
};

struct App {
  int stage{0};

  const vector<Map> maps = default_map_file_list("./maps");

  Player player{};

  int offset{0};

  AppState state{AppState::Ready};

  Countdown losingStateCountdown{180};

  App() = default;

  ~App() {
    assets.free();

    ShowCursor();
    CloseWindow();
  }

  void init() {
    InitWindow(1000, 500, "Oucher V0.1 pre-alpha dev build");
    SetTargetFPS(60);
    HideCursor();

    assets.preloadTextures("./assets/images");

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
    } else if (state == AppState::Losing) {
      losingStateCountdown.update();
      if (losingStateCountdown.isComplete()) {
        handle_losing_to_ready();
      }
    }
  }

  void update_game() {
    auto distanceFromGround = currentMap().deltaYPointToSurface(player.pos);
    player.update(distanceFromGround);

    if (player.pos.x >= currentMap().w - player.width()) handle_win();

    if (player.isDead()) handle_losing();
    if (currentMap().hasObstacleCollision(player.frame())) handle_losing();
  }

  void draw() {
    if (state == AppState::Running) {
      draw_game();
    } else if (state == AppState::Ready) {
      draw_menu();
    } else if (state == AppState::Losing) {
      draw_game();
      Text::build("GAME OVER").toCenter().withColor(RED).toLarge().draw();
    }
  }

  void draw_menu() {
    Text::build(TextFormat("Stage [%d] - Press [Enter]", stage))
        .toCenter()
        .draw();
  }

  void draw_game() const {
    currentMap().draw(xOffset());
    player.draw(xOffset());

    // DrawFPS(4, 4);
  }

  void handle_win() {
    state = AppState::Ready;
    stage = (stage + 1) % maps.size();

    player.reset();
  }

  void handle_losing() {
    state = AppState::Losing;
    losingStateCountdown.restart();
  }

  void handle_losing_to_ready() {
    state = AppState::Ready;
    stage = 0;
    player.reset();
  }

  int xOffset() const {
    if (currentMap().w < GetScreenWidth()) return 0;
    if (player.pos.x < APP_SCROLL_START_PADDING) return 0;

    return min((int)player.pos.x - APP_SCROLL_START_PADDING,
               currentMap().w - GetScreenWidth());
  }

  const Map& currentMap() const { return maps.at(stage); }
};
