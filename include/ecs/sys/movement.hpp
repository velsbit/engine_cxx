#pragma once
#include "ecs/sys/system.hpp"

namespace sys {

class MovementSystem : public ISystem {
 public:
  void update(Scene& scene, const float ts) override;
};

}  // namespace sys
