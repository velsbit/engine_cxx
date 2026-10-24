#pragma once

#include <memory>
#include <vector>

#include "ecs/sys/system.hpp"
#include "scene/scene.hpp"

namespace game {

bool init();
void shutdown();

void update(const float ts);
void render(const float alpha, const float aspect);

} // namespace game
