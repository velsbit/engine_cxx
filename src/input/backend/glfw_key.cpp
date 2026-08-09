#include "input/device.hpp"
#include "input/key.hpp"
#include <GLFW/glfw3.h>

namespace inp {

KeyCode to_keycode(Key key) noexcept {
  switch (key) {
  case Key::W:
    return KeyCode{static_cast<uint16_t>(GLFW_KEY_W)};
  case Key::A:
    return KeyCode{static_cast<uint16_t>(GLFW_KEY_A)};
  case Key::S:
    return KeyCode{static_cast<uint16_t>(GLFW_KEY_S)};
  case Key::D:
    return KeyCode{static_cast<uint16_t>(GLFW_KEY_D)};
  default:
    return KeyCode{static_cast<uint16_t>(GLFW_KEY_UNKNOWN)};
  }
}

} // namespace inp