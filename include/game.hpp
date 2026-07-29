#pragma once

#include <memory>
#include <vector>

#include "scene/scene.hpp"
#include "sys/system.hpp"

class Game {
 public:
  Game() = default;
  ~Game() = default;

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;
  Game(Game&&) = delete;
  Game& operator=(Game&&) = delete;

  bool init();
  void shutdown();

  void update(const float ts);
  void render(const float alpha, const float aspect);

 private:
  std::unique_ptr<Scene> m_active_scene;
  std::vector<std::unique_ptr<sys::ISystem>> m_systems;
  std::vector<std::unique_ptr<sys::IRenderSystem>> m_render_systems;

  void register_systems();
};