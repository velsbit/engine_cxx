#pragma once

#include <scene/scene.hpp>

namespace sys {

class ISystem {
 public:
  virtual ~ISystem() = default;

  virtual void update(Scene& scene, const float ts) = 0;
};

class IRenderSystem {
 public:
  virtual ~IRenderSystem() = default;

  virtual void update(Scene& scene, const float alpha) = 0;
};

}  // namespace sys