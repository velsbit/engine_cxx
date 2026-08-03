// input/input.hpp

#pragma once

#include "input/device.hpp"

namespace win {
struct Window;
}

namespace inp {

// Lifetime

void init(const win::Window& window) noexcept;
void shutdown() noexcept;

void begin_frame() noexcept;
void end_frame() noexcept;

// Backend events

void on_key_event(KeyCode key, bool down) noexcept;

// void on_mouse_button_event(MouseButton button, bool down) noexcept;
// void on_mouse_move(float x, float y) noexcept;
// void on_mouse_wheel(float x, float y) noexcept;
//
// void on_gamepad_button_event(...);
// void on_gamepad_axis_event(...);

}  // namespace inp