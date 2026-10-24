#pragma once

#include <entt/entt.hpp>

struct Tilemap {
  int width = 0;
  int height = 0;
  std::vector<int> tiles;
};

struct Canvas {
};

struct Scene {
  entt::registry registry;
  Tilemap tilemap;
  Canvas canvasUI;

  std::string name;
  float time_accumulator = 0.0f;
};
