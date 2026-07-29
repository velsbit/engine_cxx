#pragma once

#include <entt/entt.hpp>

class Scene {
 public:
  Scene() = default;
  ~Scene() = default;

  entt::registry& get_registry() { return m_registry; }
  const entt::registry& get_registry() const { return m_registry; }

 private:
  entt::registry m_registry;
};