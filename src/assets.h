#pragma once

#include <map>
#include <streambuf>
#include <string>
#include <vector>

#include "debug.h"
#include "raylib.h"

using namespace std;

struct Assets {
  map<string, Texture2D> textures{};

  void preloadTextures(const char* folder) {
    LOG("Loading images from %s", folder);

    FilePathList files = LoadDirectoryFiles(folder);

    for (int i = 0; i < (int)files.count; i++) {
      LOG("- image loading: %s => %s", GetFileNameWithoutExt(files.paths[i]),
          files.paths[i]);

      textures.insert(
          {GetFileNameWithoutExt(files.paths[i]), LoadTexture(files.paths[i])});
    }
  }

  void free() {
    for (auto& [_key, texture] : textures) {
      UnloadTexture(texture);
    }

    textures.clear();
  }

  const Texture2D* texture(const char* name) const {
    return &textures.at(name);
  }
};

static Assets assets = Assets();
