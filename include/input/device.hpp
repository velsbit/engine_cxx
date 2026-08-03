// input/device.hpp

#pragma once

#include <compare>
#include <cstdint>

namespace inp {

// Keyboard

struct KeyCode {
  uint16_t value = 0;

  constexpr auto operator<=>(const KeyCode&) const = default;
};

struct ScanCode {
  uint16_t value = 0;

  constexpr auto operator<=>(const ScanCode&) const = default;
};

// Mouse

struct MouseButton {
  uint8_t value = 0;

  constexpr auto operator<=>(const MouseButton&) const = default;
};

// Gamepad

struct GamepadButton {
  uint16_t value = 0;

  constexpr auto operator<=>(const GamepadButton&) const = default;
};

struct GamepadAxis {
  uint16_t value = 0;

  constexpr auto operator<=>(const GamepadAxis&) const = default;
};

}  // namespace inp