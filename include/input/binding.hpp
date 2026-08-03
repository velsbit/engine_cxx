// input/bind.hpp

#pragma once

#include <cstdint>
#include <span>
#include <vector>

#include "input/action.hpp"
#include "input/device.hpp"


namespace inp {

void bind(KeyCode key, Action action) noexcept;
void unbind(KeyCode key, Action action) noexcept;
void clear_binding(KeyCode key) noexcept;

void clear_binding(Action action) noexcept;
void clear_binding_all() noexcept;

[[nodiscard]] bool bound(KeyCode key, Action action) noexcept;

std::span<const KeyCode> get_keys(Action) noexcept;

std::span<const Action> get_actions(KeyCode);

}  // namespace inp