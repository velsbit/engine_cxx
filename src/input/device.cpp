// src/input/device.cpp

#include <array>
#include <cstdint>

#include "device_internal.hpp"

namespace inp {

namespace {

constexpr size_t kMaxKeyCodes = 512;

enum : uint8_t {
  Down = 1 << 0,
  Pressed = 1 << 1,
  Released = 1 << 2,
};

using KeyState = uint8_t;

std::array<KeyState, kMaxKeyCodes> g_keys{};
// std::array<KeyState, kMaxScanCodes> g_scancodes{};
// std::array<KeyState, kMaxMouseButtons> g_mouse_buttons{};
// std::array<KeyState, kMaxGamepadButtons> g_gamepad_buttons{};

constexpr bool is_valid_key(KeyCode key) noexcept {
  return key.value < kMaxKeyCodes;
}

}  // namespace

void set_key(KeyCode key, bool down) noexcept {
  if (!is_valid_key(key)) return;

  auto& state = g_keys[key.value];

  const bool was_down = (state & Down) != 0;

  if (was_down == down) return;

  if (down) {
    state |= Down;
    state |= Pressed;
  } else {
    state &= ~Down;
    state |= Released;
  }
}

void update_devices() noexcept {
  for (auto& state : g_keys) {
    state &= ~(Pressed | Released);
  }
}

bool key_down(KeyCode key) noexcept {
  if (!is_valid_key(key)) return false;

  return (g_keys[key.value] & Down) != 0;
}

bool key_pressed(KeyCode key) noexcept {
  if (!is_valid_key(key)) return false;

  return (g_keys[key.value] & Pressed) != 0;
}

bool key_released(KeyCode key) noexcept {
  if (!is_valid_key(key)) return false;

  return (g_keys[key.value] & Released) != 0;
}

}  // namespace inp