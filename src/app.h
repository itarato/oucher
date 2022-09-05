#pragma once

#include <string>
#include <utility>

#include "assets.h"
#include "defs.h"
#include "map.h"
#include "player.h"
#include "raylib.h"
#include "text.h"
#include "util.h"

#define APP_SCROLL_START_PADDING 200

using namespace std;

vector<shared_ptr<Map>> default_map_file_list(const char* folder) {
  FilePathList files = LoadDirectoryFiles(folder);

  vector<shared_ptr<Map>> out{};

  for (int i = 0; i < (int)files.count; i++) {
    out.emplace_back(make_shared<Map>(files.paths[i]));
    LOG("Map file loaded: %s", files.paths[i]);
  }

  return out;
}

enum class AppState {
  Ready,
  Running,
  Losing,
  Pause,
};

struct App {
  int stage{0};

  const vector<shared_ptr<Map>> maps = default_map_file_list("./maps");

  Player player;

  int offset{0};

  AppState state{AppState::Ready};

  Countdown losingStateCountdown{180};

  Shader shader;
  int groundShaderOffsetLoc{};
  int groundShaderSurfaceYsLoc{};

  shared_ptr<Map> map;

  App() : player(&map) {}

  ~App() {
    assets.free();
    CloseWindow();
  }

  void init() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Oucher V0.1 pre-alpha dev build");
    SetTargetFPS(FPS);

    assets.preloadTextures("./assets/images");

    shader = LoadShader(0, "./assets/shaders/ground.fs");
    groundShaderOffsetLoc = GetShaderLocation(shader, "x_offset");
    groundShaderSurfaceYsLoc = GetShaderLocation(shader, "surface_ys");

    restart();
  }

  void restart() {
    stage = 0;
    map = maps[0];

    player.reset();

    state = AppState::Ready;
  }

  void loop() {
    while (!WindowShouldClose()) {
      update();

      BeginDrawing();
      ClearBackground(BLACK);

      int offset = xOffset();
      float* surfaceYs = map->surfaceCache.data();
      SetShaderValue(shader, groundShaderOffsetLoc, &offset,
                     SHADER_UNIFORM_INT);
      SetShaderValueV(shader, groundShaderSurfaceYsLoc, surfaceYs,
                      SHADER_UNIFORM_FLOAT, map->surfaceCache.size());

      BeginShaderMode(shader);
      draw_shader_mode();
      EndShaderMode();

      draw();
      EndDrawing();
    }
  }

  void update() {
    if (IsKeyPressed(KEY_R)) restart();

    if (state == AppState::Running) {
      update_game();
    } else if (state == AppState::Ready) {
      if (IsKeyPressed(KEY_ENTER)) {
        state = AppState::Running;
      }
    } else if (state == AppState::Losing) {
      losingStateCountdown.update();
      update_game();
      if (losingStateCountdown.isComplete() || IsKeyPressed(KEY_ENTER)) {
        handle_losing_to_ready();
      }
    } else if (state == AppState::Pause) {
      if (IsKeyPressed(KEY_N)) {
        update_game();
      }
      if (IsKeyPressed(KEY_PAUSE)) {
        state = AppState::Running;
      }
    }
  }

  void update_game() {
    player.update();

    if (state == AppState::Running && player.pos.x >= map->w - player.width()) {
      handle_win();
    }
    if (map->hasObstacleCollision(player.frame())) player.kill();
    if (state == AppState::Running && player.isDead()) handle_losing();

    if (IsKeyPressed(KEY_PAUSE)) {
      state = AppState::Pause;
    }
  }

  void draw() {
    if (state == AppState::Running) {
      draw_game();
    } else if (state == AppState::Ready) {
      draw_menu();
    } else if (state == AppState::Losing) {
      draw_game();
      Text::build("GAME OVER").toCenter().withColor(RED).toLarge().draw();
    } else if (state == AppState::Pause) {
      draw_game();
      Text::build("PAUSED").toCenter().withColor(MAGENTA).toLarge().draw();
    }
  }

  void draw_menu() {
    Text::build(TextFormat("Stage [%d] - Press [Enter]", stage))
        .withColor(LIGHTGRAY)
        .toCenter()
        .draw();
  }

  void draw_game() const {
    map->draw_not_ground(xOffset());
    player.draw(xOffset());

    DrawFPS(4, 4);
  }

  void draw_shader_mode() const {
    if (state == AppState::Running) {
      draw_game_shader_mode();
    } else if (state == AppState::Ready) {
    } else if (state == AppState::Losing) {
      draw_game_shader_mode();
    } else if (state == AppState::Pause) {
      draw_game_shader_mode();
    }
  }

  void draw_game_shader_mode() const {
    ClearBackground(SKYBLUE);
    map->draw_ground(xOffset());
  }

  void handle_win() {
    state = AppState::Ready;
    stage = (stage + 1) % maps.size();
    map = maps[stage];

    player.reset();
  }

  void handle_losing() {
    state = AppState::Losing;
    losingStateCountdown.restart();
  }

  void handle_losing_to_ready() {
    state = AppState::Ready;
    stage = 0;
    map = maps[0];

    player.reset();
  }

  int xOffset() const {
    if (map->w < GetScreenWidth()) return 0;
    if (player.pos.x < APP_SCROLL_START_PADDING) return 0;

    return min((int)player.pos.x - APP_SCROLL_START_PADDING,
               map->w - GetScreenWidth());
  }
};
