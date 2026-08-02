#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "core/window.hpp"

namespace inp {

using ScanCode = uint16_t;
constexpr ScanCode invalidScanCode = UINT16_MAX;

using KeyCode = uint16_t;
constexpr KeyCode invalidKeyCode = UINT16_MAX;

using Action = uint32_t;
constexpr Action invalidAction = UINT32_MAX;

enum class Modifier : uint8_t {
  None = 0,
  Shift = 1 << 0,
  Ctrl = 1 << 1,
  Alt = 1 << 2,
  Super = 1 << 3,
  Any = 1 << 7
};

constexpr Modifier operator|(Modifier lhs, Modifier rhs) noexcept {
  return static_cast<Modifier>(static_cast<uint8_t>(lhs) |
                               static_cast<uint8_t>(rhs));
}

constexpr Modifier operator&(Modifier lhs, Modifier rhs) noexcept {
  return static_cast<Modifier>(static_cast<uint8_t>(lhs) &
                               static_cast<uint8_t>(rhs));
}

constexpr Modifier operator~(Modifier value) noexcept {
  return static_cast<Modifier>(~static_cast<uint8_t>(value));
}

constexpr Modifier& operator|=(Modifier& lhs, Modifier rhs) noexcept {
  return lhs = lhs | rhs;
}

constexpr Modifier& operator&=(Modifier& lhs, Modifier rhs) noexcept {
  return lhs = lhs & rhs;
}

constexpr bool has_flag(Modifier value, Modifier flag) noexcept {
  return (value & flag) != Modifier::None;
}

void init(const win::Window& window);
void shutdown();

//------------------------------------------------------------------------------
// Actions
//------------------------------------------------------------------------------

[[nodiscard]] Action create_action(std::string_view name);

void clear_action(Action action);
void clear_action(std::string_view name);

[[nodiscard]] bool has_action(Action action);
[[nodiscard]] bool has_action(std::string_view name);

[[nodiscard]] Action find_action(std::string_view name);
[[nodiscard]] std::string_view action_name(Action action);

//------------------------------------------------------------------------------
// Key / Scan code conversion
//------------------------------------------------------------------------------

[[nodiscard]] ScanCode scancode_from_string(std::string_view name);
[[nodiscard]] std::string_view scancode_to_string(ScanCode scancode);

[[nodiscard]] KeyCode keycode_from_string(std::string_view name);
[[nodiscard]] std::string_view keycode_to_string(KeyCode keycode);

[[nodiscard]] KeyCode keycode_from_scancode(ScanCode scancode);
[[nodiscard]] ScanCode scancode_from_keycode(KeyCode keycode);

[[nodiscard]] bool is_valid_scancode(ScanCode scancode);
[[nodiscard]] bool is_valid_keycode(KeyCode keycode);

//------------------------------------------------------------------------------
// Bindings
//------------------------------------------------------------------------------

void bind_scancode(Action action, ScanCode scancode,
                   Modifier modifier = Modifier::Any);
void bind_scancode(Action action, std::string_view scancode_name,
                   Modifier modifier = Modifier::Any);

void unbind_scancode(Action action, ScanCode scancode,
                     Modifier modifier = Modifier::Any);
void unbind_scancode(Action action, std::string_view scancode_name,
                     Modifier modifier = Modifier::Any);

void bind_keycode(Action action, KeyCode keycode,
                  Modifier modifier = Modifier::Any);
void bind_keycode(Action action, std::string_view keycode_name,
                  Modifier modifier = Modifier::Any);

void unbind_keycode(Action action, KeyCode keycode,
                    Modifier modifier = Modifier::Any);
void unbind_keycode(Action action, std::string_view keycode_name,
                    Modifier modifier = Modifier::Any);

void unbind_all(Action action);

[[nodiscard]] bool is_scancode_bound(Action action, ScanCode scancode,
                                     Modifier modifier = Modifier::Any);
[[nodiscard]] bool is_keycode_bound(Action action, KeyCode keycode,
                                    Modifier modifier = Modifier::Any);

//------------------------------------------------------------------------------
// Input state
//------------------------------------------------------------------------------

void update();

[[nodiscard]] bool down(Action action);
[[nodiscard]] bool pressed(Action action);
[[nodiscard]] bool released(Action action);
[[nodiscard]] bool repeated(Action action);

//------------------------------------------------------------------------------
// Bindings serialization
//------------------------------------------------------------------------------

[[nodiscard]] bool load_bindings(std::string_view path);
[[nodiscard]] bool save_bindings(std::string_view path);

//------------------------------------------------------------------------------
// Reset
//------------------------------------------------------------------------------

void clear_bindings();
void clear_actions();

}  // namespace inp