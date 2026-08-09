#include <cstdint>

namespace inp {

struct KeyCode;
struct ScanCode;

enum class Key : uint16_t {
  None = 0,
  Escape,
  Backspace,
  Tab,
  Capslock,
  Enter,
  Space,
  PrintScreen,
  ScrollLock,
  Pause,

  // Modifier keys
  LeftShift,
  RightShift,
  LeftControl,
  RightControl,
  LeftAlt,
  RightAlt,
  LeftSuper,
  RightSuper,
  Menu,

  // Letters
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,

  // Numbers
  Alpha0,
  Alpha1,
  Alpha2,
  Alpha3,
  Alpha4,
  Alpha5,
  Alpha6,
  Alpha7,
  Alpha8,
  Alpha9,

  // Function keys
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,

  // Arrow keys
  UpArrow,
  DownArrow,
  LeftArrow,
  RightArrow,

  // Navigation keys
  Insert,
  Delete,
  Home,
  End,
  PageUp,
  PageDown,

  // Numpad keys
  NumLock,
  Numpad0,
  Numpad1,
  Numpad2,
  Numpad3,
  Numpad4,
  Numpad5,
  Numpad6,
  Numpad7,
  Numpad8,
  Numpad9,
  NumpadDivide,   // /
  NumpadMultiply, // *
  NumpadSubtract, // -
  NumpadAdd,      // +
  NumpadEnter,
  NumpadDecimal, // . ,

  // Other keys
  Backquote,    // ` ~
  Minus,        // - _
  Equals,       // = +
  LeftBracket,  // [ {
  RightBracket, // ] }
  Backslash,    // \ |
  Semicolon,    // ; :
  Quote,        // ' "
  Comma,        // , <
  Period,       // . >
  Slash         // / ?
};

KeyCode to_keycode(Key key) noexcept;
ScanCode to_scancode(Key key) noexcept;

} // namespace inp