#include "ecs/sys/system.hpp"

#include "ecs/sys/movement.hpp"

#include <algorithm>
#include <array>

namespace sys {

inline constexpr std::array g_systems{
    // Stage: Input (External)

    // Stage: AI Logic (Internal)

    // Stage: Logic & State Machines

    // Stage: Physics & Movement
    System{.name = "movement", .update = sys::movement},

    // Stage: Event Processing

    // Stage: Cleanup & Lifecycle
};

const System *find_system_by_name(std::string_view name) noexcept {
  auto it = std::ranges::find_if(g_systems, [name](const auto &sys) {
    return sys.name == name;
  });

  return (it != g_systems.end()) ? &(*it) : nullptr;
}

} // namespace sys
