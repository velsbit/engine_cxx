#include "ecs/sys/movement.hpp"

#include "ecs/cmp/position.hpp"
#include "ecs/cmp/velocity.hpp"
#include "scene/scene.hpp"

namespace sys {

void movement(Scene &scene, float ts) noexcept {
  auto view = scene.registry.view<cmp::Position, cmp::Velocity>();
  view.each([ts](cmp::Position &pos, const cmp::Velocity &vel) {
    pos.value += vel.value * ts;
  });
}

} // namespace sys
