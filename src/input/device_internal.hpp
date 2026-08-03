// src/input/device_internal.hpp

#pragma once

#include "input/device.hpp"

namespace inp {

void set_key(KeyCode key, bool down) noexcept;
void update_devices() noexcept;

bool key_down(KeyCode key) noexcept;
bool key_pressed(KeyCode key) noexcept;
bool key_released(KeyCode key) noexcept;

}  // namespace inp