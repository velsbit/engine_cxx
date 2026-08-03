#include "ecs/sys/movement.hpp"

#include "ecs/cmp/position.hpp"
#include "ecs/cmp/velocity.hpp"
#include "scene/scene.hpp"

namespace sys {

void sys::MovementSystem::update(Scene& scene, const float ts) {
  auto view = scene.get_registry().view<cmp::Position, cmp::Velocity>();
  view.each([ts](cmp::Position& pos, const cmp::Velocity& vel) {
    pos.value += vel.value * ts;
  });
}

}  // namespace sys