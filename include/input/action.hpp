// input/action.hpp

#pragma once

#include <cstdint>
#include <string_view>

namespace inp {

struct Action {
  uint16_t index{0};
  uint16_t gen{0};

  constexpr auto operator<=>(const Action&) const = default;

  [[nodiscard]]
  constexpr explicit operator bool() const noexcept {
    return gen != 0;
  }
};

[[nodiscard]] Action create_action(std::string_view name) noexcept;
void destroy_action(Action) noexcept;

[[nodiscard]] bool down(Action) noexcept;
[[nodiscard]] bool pressed(Action) noexcept;
[[nodiscard]] bool released(Action) noexcept;

// utils

[[nodiscard]] std::string_view get_action_name(Action) noexcept;

}  // namespace inp