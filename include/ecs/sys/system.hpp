#pragma once

#include <cstdint>
#include <scene/scene.hpp>
#include <string_view>
#include <vector>

namespace sys {

struct System {
  std::string_view name;
  void (*update)(Scene &, float) noexcept;
};

using Pipeline = std::vector<System *>;

const System *find_system_by_name(std::string_view name) noexcept;

} // namespace sys
