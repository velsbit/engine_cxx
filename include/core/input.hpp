// core/input.hpp
#pragma once

#include <cstdint>

#include "core/window.hpp"

namespace inp {
// clang-format off
enum class Key : uint16_t {
    Unknown = 0,

    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    Num0, Num1, Num2, Num3, Num4,
    Num5, Num6, Num7, Num8, Num9,

    Space, Escape, Enter, Tab, Backspace,
    Insert, Delete, Right, Left, Down, Up,
    PageUp, PageDown, Home, End,
    CapsLock, ScrollLock, NumLock, PrintScreen, Pause,

    LeftShift, LeftControl, LeftAlt, LeftSuper,
    RightShift, RightControl, RightAlt, RightSuper,
    Menu,

    Apostrophe,   // '
    Comma,        // ,
    Minus,        // -
    Period,       // .
    Slash,        // /
    Semicolon,    // ;
    Equal,        // =
    LeftBracket,  // [
    Backslash,    // 
    RightBracket, // ]
    GraveAccent,  // `

    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10,
    F11, F12, F13, F14, F15, F16, F17, F18, F19, F20,
    F21, F22, F23, F24, F25,

    Kp0, Kp1, Kp2, Kp3, Kp4, Kp5, Kp6, Kp7, Kp8, Kp9,
    KpDecimal, KpDivide, KpMultiply, KpSubtract, KpAdd,
    KpEnter, KpEqual,

    Count
};
// clang-format on

enum class Action : uint16_t {
  MoveLeft,
  MoveRight,
  MoveUp,
  MoveDown,
  Jump,
  Shoot,

  Count
};

void init(const win::Window& window);
void shutdown();

void bind(Action action, Key key);
void unbind(Action action, Key key);

void update();
void post_fixed_update();

[[nodiscard]] bool down(Action action);
[[nodiscard]] bool pressed(Action action);
[[nodiscard]] bool released(Action action);

// [[nodiscard]] bool down(Key key);
// [[nodiscard]] bool pressed(Key key);
// [[nodiscard]] bool released(Key key);

}  // namespace inp