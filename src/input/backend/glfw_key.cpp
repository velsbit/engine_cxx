#include "input/device.hpp"
#include "input/key.hpp"
#include <GLFW/glfw3.h>

namespace inp {

// clang-format off
KeyCode to_keycode(Key key) noexcept {
  switch (key) {
  case Key::None:          return {};
  case Key::Escape:        return {GLFW_KEY_ESCAPE};
  case Key::Backspace:     return {GLFW_KEY_BACKSPACE};
  case Key::Tab:           return {GLFW_KEY_TAB};
  case Key::Capslock:      return {GLFW_KEY_CAPS_LOCK};
  case Key::Enter:         return {GLFW_KEY_ENTER};
  case Key::Space:         return {GLFW_KEY_SPACE};
  case Key::PrintScreen:   return {GLFW_KEY_PRINT_SCREEN};
  case Key::ScrollLock:    return {GLFW_KEY_SCROLL_LOCK};
  case Key::Pause:         return {GLFW_KEY_PAUSE};

  // Modifier keys
  case Key::LeftShift:     return {GLFW_KEY_LEFT_SHIFT};
  case Key::RightShift:    return {GLFW_KEY_RIGHT_SHIFT};
  case Key::LeftControl:   return {GLFW_KEY_LEFT_CONTROL};
  case Key::RightControl:  return {GLFW_KEY_RIGHT_CONTROL};
  case Key::LeftAlt:       return {GLFW_KEY_LEFT_ALT};
  case Key::RightAlt:      return {GLFW_KEY_RIGHT_ALT};
  case Key::LeftSuper:     return {GLFW_KEY_LEFT_SUPER};
  case Key::RightSuper:    return {GLFW_KEY_RIGHT_SUPER};
  case Key::Menu:          return {GLFW_KEY_MENU};

  // Letters
  case Key::A: return {GLFW_KEY_A};
  case Key::B: return {GLFW_KEY_B};
  case Key::C: return {GLFW_KEY_C};
  case Key::D: return {GLFW_KEY_D};
  case Key::E: return {GLFW_KEY_E};
  case Key::F: return {GLFW_KEY_F};
  case Key::G: return {GLFW_KEY_G};
  case Key::H: return {GLFW_KEY_H};
  case Key::I: return {GLFW_KEY_I};
  case Key::J: return {GLFW_KEY_J};
  case Key::K: return {GLFW_KEY_K};
  case Key::L: return {GLFW_KEY_L};
  case Key::M: return {GLFW_KEY_M};
  case Key::N: return {GLFW_KEY_N};
  case Key::O: return {GLFW_KEY_O};
  case Key::P: return {GLFW_KEY_P};
  case Key::Q: return {GLFW_KEY_Q};
  case Key::R: return {GLFW_KEY_R};
  case Key::S: return {GLFW_KEY_S};
  case Key::T: return {GLFW_KEY_T};
  case Key::U: return {GLFW_KEY_U};
  case Key::V: return {GLFW_KEY_V};
  case Key::W: return {GLFW_KEY_W};
  case Key::X: return {GLFW_KEY_X};
  case Key::Y: return {GLFW_KEY_Y};
  case Key::Z: return {GLFW_KEY_Z};

  // Numbers
  case Key::Alpha0: return {GLFW_KEY_0};
  case Key::Alpha1: return {GLFW_KEY_1};
  case Key::Alpha2: return {GLFW_KEY_2};
  case Key::Alpha3: return {GLFW_KEY_3};
  case Key::Alpha4: return {GLFW_KEY_4};
  case Key::Alpha5: return {GLFW_KEY_5};
  case Key::Alpha6: return {GLFW_KEY_6};
  case Key::Alpha7: return {GLFW_KEY_7};
  case Key::Alpha8: return {GLFW_KEY_8};
  case Key::Alpha9: return {GLFW_KEY_9};

  // Function keys
  case Key::F1:  return {GLFW_KEY_F1};
  case Key::F2:  return {GLFW_KEY_F2};
  case Key::F3:  return {GLFW_KEY_F3};
  case Key::F4:  return {GLFW_KEY_F4};
  case Key::F5:  return {GLFW_KEY_F5};
  case Key::F6:  return {GLFW_KEY_F6};
  case Key::F7:  return {GLFW_KEY_F7};
  case Key::F8:  return {GLFW_KEY_F8};
  case Key::F9:  return {GLFW_KEY_F9};
  case Key::F10: return {GLFW_KEY_F10};
  case Key::F11: return {GLFW_KEY_F11};
  case Key::F12: return {GLFW_KEY_F12};

  // Arrow keys
  case Key::UpArrow:    return {GLFW_KEY_UP};
  case Key::DownArrow:  return {GLFW_KEY_DOWN};
  case Key::LeftArrow:  return {GLFW_KEY_LEFT};
  case Key::RightArrow: return {GLFW_KEY_RIGHT};

  // Navigation
  case Key::Insert:   return {GLFW_KEY_INSERT};
  case Key::Delete:   return {GLFW_KEY_DELETE};
  case Key::Home:     return {GLFW_KEY_HOME};
  case Key::End:      return {GLFW_KEY_END};
  case Key::PageUp:   return {GLFW_KEY_PAGE_UP};
  case Key::PageDown: return {GLFW_KEY_PAGE_DOWN};

  // Numpad
  case Key::NumLock:       return {GLFW_KEY_NUM_LOCK};
  case Key::Numpad0:       return {GLFW_KEY_KP_0};
  case Key::Numpad1:       return {GLFW_KEY_KP_1};
  case Key::Numpad2:       return {GLFW_KEY_KP_2};
  case Key::Numpad3:       return {GLFW_KEY_KP_3};
  case Key::Numpad4:       return {GLFW_KEY_KP_4};
  case Key::Numpad5:       return {GLFW_KEY_KP_5};
  case Key::Numpad6:       return {GLFW_KEY_KP_6};
  case Key::Numpad7:       return {GLFW_KEY_KP_7};
  case Key::Numpad8:       return {GLFW_KEY_KP_8};
  case Key::Numpad9:       return {GLFW_KEY_KP_9};
  case Key::NumpadDivide:  return {GLFW_KEY_KP_DIVIDE};
  case Key::NumpadMultiply: return {GLFW_KEY_KP_MULTIPLY};
  case Key::NumpadSubtract: return {GLFW_KEY_KP_SUBTRACT};
  case Key::NumpadAdd:      return {GLFW_KEY_KP_ADD};
  case Key::NumpadEnter:    return {GLFW_KEY_KP_ENTER};
  case Key::NumpadDecimal:  return  {GLFW_KEY_KP_DECIMAL};

  // Other
  case Key::Backquote:    return {GLFW_KEY_GRAVE_ACCENT};
  case Key::Minus:        return {GLFW_KEY_MINUS};
  case Key::Equals:       return {GLFW_KEY_EQUAL};
  case Key::LeftBracket:  return {GLFW_KEY_LEFT_BRACKET};
  case Key::RightBracket: return {GLFW_KEY_RIGHT_BRACKET};
  case Key::Backslash:    return {GLFW_KEY_BACKSLASH};
  case Key::Semicolon:    return {GLFW_KEY_SEMICOLON};
  case Key::Quote:        return {GLFW_KEY_APOSTROPHE};
  case Key::Comma:        return {GLFW_KEY_COMMA};
  case Key::Period:       return {GLFW_KEY_PERIOD};
  case Key::Slash:        return {GLFW_KEY_SLASH};
  }

  return {static_cast<uint16_t>(GLFW_KEY_UNKNOWN)};
}
// clang-format on

} // namespace inp